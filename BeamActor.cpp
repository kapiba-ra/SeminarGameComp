#include "BeamActor.h"
#include "GamePlay.h"
#include "HpComponent.h"
#include "PlayerActor.h"

// DEBUG: 静的フラグ定義
bool BeamActor::sDebugBeam = true; // DEBUG

//--------------------------------------
BeamActor::BeamActor(GamePlay* gp, Vector2 start, int forward)
: Actor(gp, Actor::Eweapon), mGP(gp)
{
    setForward(forward);
    setPosition(start);
    computeRectangle();

    // 見た目用テクスチャ（管理側APIに合わせてください）
    mArrowTex = mGP->getTexture("Assets/testArrow.png");
    mArrowTexValid = (mArrowTex != nullptr && mArrowTex->id != 0);
}

BeamActor::~BeamActor() {}

//--------------------------------------
// FIX: 左端中央基準で向き対称にAABBを計算
void BeamActor::computeRectangle() // FIX
{
    const float halfH = mBeamH * 0.5f;

    if (mForward > 0) {
        // 右向き：左端が position.x
        mRectangle = { getPosition().x,          getPosition().y - halfH, mBeamW, mBeamH };
    } else {
        // 左向き：右端が position.x
        mRectangle = { getPosition().x - mBeamW, getPosition().y - halfH, mBeamW, mBeamH };
    }
}

//--------------------------------------
// DEBUG: ヒット状態を覚えて色分け
void BeamActor::updateDamage()
{
    mHitThisFrame = false; // DEBUG: フレーム頭でリセット

    PlayerActor* player = mGP->getPlayer();
    if (!player) return;

    if (CheckCollisionRecs(player->getRectangle(), mRectangle)) {
        mHitThisFrame = true; // DEBUG
        if (auto* hp = player->getHpComp()) {
            hp->TakeDamage(18.0f);
        }
    }
}

//--------------------------------------
void BeamActor::updateVisualArrow(float dt)
{
    if (!mArrowTexValid) return;

    // 0→1 で横移動（ループさせたいなら while(mT>1) mT-=1; に変更）
    mT += dt * mVisSpeed;
    if (mT > 1.0f) mT = 1.0f;

    const float xStart = (mForward >= 0)
        ? getPosition().x
        : (getPosition().x - mBeamW);

    const float x = (mForward >= 0)
        ? (xStart + (mBeamW * mT))
        : (xStart + (mBeamW * (1.0f - mT)));

    mVisPos = { x, getPosition().y };
}

//--------------------------------------
void BeamActor::update()
{
    Actor::update();
    const float dt = GetFrameTime();

    // DEBUG: Lキーで全インスタンス共通の可視化トグル
    if (IsKeyPressed(KEY_L)) {            // DEBUG
        sDebugBeam = !sDebugBeam;         // DEBUG
    }

    // 寿命
    mLife -= dt;
    if (mLife <= 0.0f) {
        setState(Actor::Edead);
        return;
    }

    // 判定／ダメージ／見た目更新
    computeRectangle();
    updateDamage();
    updateVisualArrow(dt);

    draw();
}

//--------------------------------------
void BeamActor::draw() const
{
    // FIX: テクスチャ無効でもデバッグは描くため、早期returnしない
    if (mArrowTexValid) { // 見た目は有効時のみ描画
        const float texW = static_cast<float>(mArrowTex->width);
        const float texH = static_cast<float>(mArrowTex->height);

        Rectangle src = { 0, 0, (mForward >= 0 ? texW : -texW), texH };
        Rectangle dst = { mVisPos.x, mVisPos.y, texW, mBeamH };
        Vector2   origin = { 0.0f, mBeamH * 0.5f };

        DrawTexturePro(*mArrowTex, src, dst, origin, 0.0f, WHITE);
    }

    // DEBUG: 当たりの可視化（ON時のみ）
    if (sDebugBeam) { // DEBUG
        const Color fill = mHitThisFrame ? Color{0,255,0,80} : Color{255,0,0,80}; // DEBUG
        const Color line = mHitThisFrame ? GREEN : RED;                            // DEBUG

        DrawRectangleRec(mRectangle, fill);                 // DEBUG: 半透明塗り
        DrawRectangleLinesEx(mRectangle, 2.0f, line);       // DEBUG: 外枠

        const float cy = mRectangle.y + mRectangle.height * 0.5f; // DEBUG: 中央ライン
        DrawLineEx({ mRectangle.x, cy },
                   { mRectangle.x + mRectangle.width, cy },
                   1.0f, (mHitThisFrame ? DARKGREEN : MAROON));    // DEBUG

        // 原点マーカー（左端中央）
        const Vector2 originL = { (mForward > 0) ? mRectangle.x : (mRectangle.x + mRectangle.width),
                                  mRectangle.y + mRectangle.height * 0.5f };
        DrawCircleV(originL, 4.0f, line); // DEBUG
    }
}
