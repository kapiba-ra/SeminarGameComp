#include "ArrowComponent.h"
#include "ArrowActor.h"
#include "Actor.h"
#include <algorithm>

ArrowComponent::ArrowComponent(Actor* owner)
    : WeaponComponent(owner)
{
}

void ArrowComponent::update()
{
    // 終了フラグが立った矢の参照を掃除
    cullArrows();
}

void ArrowComponent::startAttack(AttackType type)
{
    spawnArrow();
}

void ArrowComponent::spawnArrow()
{
    auto* seq = mOwner->getSequence();
    auto* arrow = new ArrowActor(seq, mOwner->getType());

    // 向き
    arrow->setForward(mOwner->getForward());

    // 位置（所有者の少し前）
    Vector2 pos = mOwner->getPosition();
    Rectangle ownerRect = mOwner->getRectangle();
    Rectangle wepRect   = arrow->getRectangle(); // ArrowActorのctorで初期化済み

    pos.x += (ownerRect.width * 0.5f + wepRect.width * 0.5f + mMuzzleOffset)
             * arrow->getForward();
    arrow->setPosition(pos);

    // 速度：左右のみ（必要ならY方向/重力を足す）
    float speed = mBaseSpeed;
    Vector2 vel{ speed * arrow->getForward(), 0.0f };
    arrow->setVelocity(vel);

    // 寿命
    arrow->setLifeTime(mLifeTime);

    // アニメ開始
    arrow->onStartAttack();

    // 当たり再計算
    arrow->computeRectangle();

    // 管理に追加
    mArrows.push_back(arrow);

    // WeaponComponent互換：最後に撃った矢をmWeaponとして持っておく（任意）
    mWeapon = arrow;
}

void ArrowComponent::cullArrows()
{
    // この関数は「参照の掃除」だけ。実体の破棄は
    // あなたのActor/Sequence管理（GC/自動削除）規約に合わせて実装してください。
    mArrows.erase(
        std::remove_if(mArrows.begin(), mArrows.end(),
            [](ArrowActor* a){ return a->isDead(); }),
        mArrows.end()
    );

    if (mWeapon) {
        auto* last = static_cast<ArrowActor*>(mWeapon);
        if (last->isDead()) mWeapon = nullptr;
    }
}
