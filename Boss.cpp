//===============================================
// Boss.cpp
//===============================================
#include "Boss.h"
#include "GamePlay.h"
#include "PlayerActor.h"
#include "SpriteComponent.h"
#include "AnimSpriteComponent.h"
#include "AttackComponent.h"
#include "GuardComponent.h"
#include "HpComponent.h"
#include "LobProjectile.h"
#include "BeamActor.h"
#include "MinionActor.h"
#include "BossMove.h"
#include <raylib.h>
#include "SoundSystem.h"

// 数値調整（必要に応じて変更）
static const float kBossHpMax = 400.0f;
static const float kGuardMax = 180.0f;
static const float kGuardBreakInvincSec = 0.35f; // ガードブレイク直後のガード無敵
static const float kHpLeakWhileGuard = 0.20f; // ガード中のHPリーク(10%)

// ダメージタグ別のガード係数（小さいほどよく減る）
static const float kGuardCoefNormal = 1.0f;
static const float kGuardCoefMeleeLight = 0.9f;
static const float kGuardCoefMeleeHeavy = 0.7f;
static const float kGuardCoefLob = 0.8f;
static const float kGuardCoefBeam = 1.1f;
static const float kGuardCoefExplosion = 0.35f; // 爆発に弱い

// 攻撃CD
static const float kCdMeleeLight = 1.2f;
static const float kCdMeleeHeavy = 2.2f;
static const float kCdLob = 1.6f;
static const float kCdBeam = 3.0f;

// 召喚
static const float kSummonInterval = 8.0f;
static const int   kSummonCount = 3;
static const float kMinionLifeSeconds = 8.0f;
static const float kMinionHealAmount = 15.0f;

Boss::Boss(GamePlay* seq)
    : EnemyActor(seq, EnemyActor::Type::Boss)
{
    // HP/ガード初期化
    mHpComp = new HpComponent(this, kBossHpMax, 0.0f);
    mGuard = new GuardComponent(this, kGuardMax, kGuardBreakInvincSec);

    // 基本見た目（暫定・必要なら差し替え）
    if (auto* gp = static_cast<GamePlay*>(getSequence())) {
        Texture2D* tex = gp->getTexture("Assets/Boss.png");
        if (tex) {
            auto* spr = new SpriteComponent(this);
            spr->setTexture(*tex);
        }
    }

    // 動き
    new BossMove(this);

    // 配置（最奥寄り）: GamePlay 側で setPosition 済みならそのまま
    // mPosition は GamePlay::onEnterBossArea で上書きされます
    computeRectangle();
}

void Boss::update() {
    EnemyActor::update();

    const float dt = GetFrameTime();
    // クールダウン経過
    if (mMeleeLightCd > 0) mMeleeLightCd -= dt;
    if (mMeleeHeavyCd > 0) mMeleeHeavyCd -= dt;
    if (mLobCd > 0) mLobCd -= dt;
    if (mBeamCd > 0) mBeamCd -= dt;

    tryGuardRecharge();
    tryAttacks(dt);
    trySummon(dt);
}

void Boss::ApplyDamage(float dmg, DamageTag tag) {
    // ガードへの係数適用
    float coef = kGuardCoefNormal;
    switch (tag) {
    case DamageTag::MeleeLight:    coef = kGuardCoefMeleeLight; break;
    case DamageTag::MeleeHeavy:    coef = kGuardCoefMeleeHeavy; break;
    case DamageTag::ProjectileLob: coef = kGuardCoefLob;        break;
    case DamageTag::Beam:          coef = kGuardCoefBeam;       break;
    case DamageTag::Explosion:     coef = kGuardCoefExplosion;  break;
    default:                       coef = kGuardCoefNormal;     break;
    }

    if (mGuard && mGuard->HasGuard()) {
        // ガードを減らし、HPへは少量だけ通す
        mGuard->TakeGuardDamage(dmg * coef, tag);
        if (mHpComp) mHpComp->TakeDamage(dmg * kHpLeakWhileGuard);
    }
    else {
        if (mHpComp) if (mHpComp->TakeDamage(dmg)) {
            setState(Edead);
            SoundSystem::instance().stopBGM();
            static_cast<GamePlay*>(mSequence)->onBossKilled();
        }
    }
}

void Boss::Heal(float hp) {
    if (mHpComp) mHpComp->Recover(hp);
}

void Boss::tryGuardRecharge() {
    if (!mHpComp || !mGuard) return;
    const float ratio = mHpComp->GetHpRatio();
    if (!mDidHalfRecharge && ratio <= 0.5f) {
        mGuard->RechargeFull();
        mDidHalfRecharge = true;
    }
    if (!mDidQuarterRecharge && ratio <= 0.25f) {
        mGuard->RechargeFull();
        mDidQuarterRecharge = true;
    }
}

void Boss::tryAttacks(float dt) {
    auto* gp = static_cast<GamePlay*>(getSequence());
    if (!gp) return;
    auto* player = gp->getPlayer();
    if (!player) return;

    const float dx = player->getPosition().x - mPosition.x;
    const float dist = fabsf(dx);
    const float meleeRange = 80.0f;   // 近接有効レンジ

    // 近接：レンジ内なら優先
    if (dist <= meleeRange) {
        // 前方に矩形を出して当たり判定（重→軽の優先）
        const float offX = (mForward > 0 ? 32.0f : -92.0f); // 前方向へ
        const Rectangle heavyHit{ mPosition.x + offX, mPosition.y - 40.0f, 60.0f, 60.0f };
        const Rectangle lightHit{ mPosition.x + offX, mPosition.y - 36.0f, 56.0f, 56.0f };

        if (mMeleeHeavyCd <= 0.0f) {
            if (CheckCollisionRecs(heavyHit, player->getRectangle())) {
                if (player->getPlayerState()->getType() == PlayerState::Type::Dodge) return;
                player->getHpComp()->TakeDamage(35.0f);
            }
            mMeleeHeavyCd = kCdMeleeHeavy;
            return;
        }

        if (mMeleeLightCd <= 0.0f) {
            if (CheckCollisionRecs(lightHit, player->getRectangle())) {
                if (player->getPlayerState()->getType() == PlayerState::Type::Dodge) return;
                player->getHpComp()->TakeDamage(18.0f);
            }
            mMeleeLightCd = kCdMeleeLight;
            return;
        }
    }

    // 遠距離：レンジ外なら射撃
    if (mLobCd <= 0.0f) {
        // 放物線投擲
        auto* lob = new LobProjectileActor(gp, mPosition, (dx >= 0 ? +1 : -1));
        (void)lob;
        mLobCd = kCdLob;
    }

    if (mBeamCd <= 0.0f) {
        // 直線ビーム/魔法
        auto* beam = new BeamActor(
            gp,
            Vector2{ mPosition.x + (mForward > 0 ? 32.0f : -32.0f), mPosition.y },
            mForward
        );
        (void)beam;
        mBeamCd = kCdBeam;
    }
}

void Boss::trySummon(float dt) {
    mSummonTimer += dt;
    if (mSummonTimer >= kSummonInterval) {
        mSummonTimer = 0.0f;
        auto* gp = static_cast<GamePlay*>(getSequence());
        if (!gp) return;
        for (int i = 0; i < kSummonCount; ++i) {
            auto* m = new MinionActor(gp, this, kMinionLifeSeconds, kMinionHealAmount);
            m->setPosition({ mPosition.x - 120.0f - 40.0f * i, mPosition.y });
        }
    }
}

void Boss::computeRectangle()
{
    const float w = 64.0f;
    const float h = 128.0f;
    mRectangle = {
        mPosition.x - w * 0.5f,
        mPosition.y - h * 0.5f,
        w,
        h
    };
}
