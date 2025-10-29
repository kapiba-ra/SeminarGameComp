//===============================================
// BossMove.cpp
//  EnemyMove を参考にした床/段差/壁を考慮したシンプル歩行AI
//===============================================
#include "BossMove.h"
#include "Boss.h"
#include "GamePlay.h"
#include "Stage.h"
#include "PlayerActor.h"

BossMove::BossMove(Boss* owner)
    : MoveComponent(owner)
    , mBoss(owner)
    , mGP(static_cast<GamePlay*>(owner->getSequence()))
    , mPlayer(mGP->getPlayer())
    , mKeepRange(220.0f)
    , mTileSize(32)  // Stage のタイルサイズに合わせる
{
    // 既定値（EnemyMove 近似）
    mGravity   = 1800.0f;
    mMoveSpeed = 120.0f;
    mJumpSpeed = -600.0f;
}

void BossMove::update() {
    if (!mBoss || mBoss->getState() != Actor::Ealive) return;

    const float dt = GetFrameTime();

    // --- 追尾：プレイヤーに向かって歩く（一定距離で止まる） ---
    const Rectangle brec = mBoss->getRectangle();
    const Rectangle prec = mPlayer->getRectangle();
    const float myX  = brec.x + brec.width * 0.5f;
    const float pX   = prec.x + prec.width * 0.5f;
    const float dx   = pX - myX;
    const float adx  = fabsf(dx);

    // 距離が開いていれば寄る。近ければ停止
    if (adx > mKeepRange) {
        mVelocityX = (dx > 0 ? +mMoveSpeed : -mMoveSpeed);
        mBoss->setForward(dx > 0 ? +1 : -1);
    } else {
        mVelocityX = 0.0f;
        // 停止中も向きはプレイヤーへ
        if (dx != 0) mBoss->setForward(dx > 0 ? +1 : -1);
    }

    // --- 重力 ---
    mVelocityY += mGravity * dt;
    if (mVelocityY > 2000.0f) mVelocityY = 2000.0f; // 安全クランプ

    // --- 位置を一旦更新（後で衝突で補正） ---
    Vector2 pos = mBoss->getPosition();
    pos.y += mVelocityY * dt;
    mBoss->setPosition(pos);
    mBoss->computeRectangle();

    // ステージ衝突（縦方向）
    const auto stageRecs = mGP->getStage()->getStageRecs();
    resolveVertical(stageRecs);

    // 横方向（壁/段差）
    pos = mBoss->getPosition();
    pos.x += mVelocityX * dt;
    mBoss->setPosition(pos);
    mBoss->computeRectangle();

    resolveHorizontal(stageRecs);

    // 最終的な当たり更新
    mBoss->computeRectangle();
}

void BossMove::fixFloorCol() {
    // 着地：落下速度をリセット
    mVelocityY = 0.0f;
}

void BossMove::resolveVertical(const std::vector<Rectangle>& stageRecs) {
    // 下方向への落下で床に乗る
    Rectangle rec = mBoss->getRectangle();
    for (const auto& s : stageRecs) {
        if (CheckCollisionRecs(rec, s)) {
            // 上から接地
            const float bottom = rec.y + rec.height;
            if (bottom <= s.y + mTileSize * 1.5f && mVelocityY >= 0.0f) {
                // 足元に吸着
                const float newY = s.y - rec.height;
                mBoss->setPosition({ rec.x + rec.width * 0.5f, newY + rec.height * 0.5f });
                mBoss->computeRectangle();
                fixFloorCol();
                return;
            }
            // 上面衝突（頭ぶつけ）
            if (rec.y >= s.y + s.height - 1.0f && mVelocityY < 0.0f) {
                const float newY = s.y + s.height;
                mBoss->setPosition({ rec.x + rec.width * 0.5f, newY + rec.height * 0.5f });
                mBoss->computeRectangle();
                mVelocityY = 0.0f;
                return;
            }
        }
    }
}

bool BossMove::tryStepUp(const Rectangle& stageRec, const std::vector<Rectangle>& stageRecs, float tileSize) {
    // WeakEnemy と同コンセプト：低い段差で“乗り上げ”を試みる
    // 「進行方向の先に段差がある」「段差高さ <= 1.5 * tile」「1タイル上が空間ならOK」
    const Rectangle colRec = mBoss->getRectangle();

    bool isStep = false;
    if ((mVelocityX > 0 && colRec.x < stageRec.x) || (mVelocityX < 0 && colRec.x > stageRec.x)) {
        isStep = (stageRec.height <= tileSize * 1.5f);
    }

    if (!isStep) return false;

    Rectangle checkOneAbove = {
        colRec.x,
        stageRec.y - tileSize,
        colRec.width,
        1.0f
    };
    bool isSpaceAboveClear = true;
    for (const auto& other : stageRecs) {
        if (CheckCollisionRecs(checkOneAbove, other)) {
            isSpaceAboveClear = false; break;
        }
    }
    if (!isSpaceAboveClear) return false;

    // 乗り上げ：矩形の底面を段の上面にスナップ
    float newYTopOfStep = stageRec.y - colRec.height;
    mBoss->setPosition({ colRec.x + colRec.width * 0.5f, newYTopOfStep + colRec.height * 0.5f });
    mBoss->computeRectangle();
    // 段差に乗った＝落下扱いをリセット
    fixFloorCol();
    return true;
}

void BossMove::resolveHorizontal(const std::vector<Rectangle>& stageRecs) {
    Rectangle rec = mBoss->getRectangle();
    const float tileSize = (float)mTileSize;

    for (const auto& s : stageRecs) {
        if (!CheckCollisionRecs(rec, s)) continue;

        // まずは step up を試す
        if (tryStepUp(s, stageRecs, tileSize)) {
            rec = mBoss->getRectangle();
            continue;
        }

        // 乗り上げできない → 壁として押し戻す
        if (mVelocityX > 0.0f) {
            // 右に進んで衝突 → 右側面にスナップ
            float newX = s.x - rec.width;
            mBoss->setPosition({ newX + rec.width * 0.5f, rec.y + rec.height * 0.5f });
            mBoss->computeRectangle();
            mVelocityX = 0.0f;
            rec = mBoss->getRectangle();
        } else if (mVelocityX < 0.0f) {
            float newX = s.x + s.width;
            mBoss->setPosition({ newX + rec.width * 0.5f, rec.y + rec.height * 0.5f });
            mBoss->computeRectangle();
            mVelocityX = 0.0f;
            rec = mBoss->getRectangle();
        }
    }
}
