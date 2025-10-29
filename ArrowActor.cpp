#include "ArrowActor.h"
#include "GamePlay.h"

#include "AnimSpriteComponent.h"
#include "AttackComponent.h"

ArrowActor::ArrowActor(Sequence* sequence, Type owner)
    : WeaponActor(sequence, owner)
{
    // 初期テクスチャと当たり
    //Texture2D tex0 = mSequence->getTexture("arrow01.png");
    Texture2D* tex0 = mSequence->getTexture("Assets/arrow.png");
    mPosition = Vector2{ 500.0f, 300.0f };
    mRectangle = {
        mPosition.x - tex0->width / 2.0f,
        mPosition.y - tex0->height / 2.0f,
        (float)tex0->width,
        (float)tex0->height
    };

    // 簡易アニメ（お好みでフレーム追加）
    mAnimsc = new AnimSpriteComponent(this);

    std::vector<Texture2D*> frames = {
        tex0,
        /*mSequence->getTexture("arrow02.png"),
        mSequence->getTexture("arrow03.png"),
        */
    };
    mAnim.frames = frames;
    mAnim.loop = true;

    // 攻撃
    mAttackComp = new AttackComponent(this);
    
    mAttackInfo.damage = 15.0f;
    mAttackInfo.duration = 1.0f;
    mAttackInfo.colRect = mRectangle;
    mAttackInfo.knockBack = 0.0f;
    mAttackInfo.targetMask = Actor::Type::Eplayer;
    
    mAttackComp->startAttack(&mAttackInfo);
    mAnimsc->play(&mAnim); // 矢は回転/ヒラヒラ想定でループ
}

void ArrowActor::input()
{
    Actor::input();
}

void ArrowActor::update()
{
    Actor::update();

    if (mDead) return;

    const float dt = GetFrameTime();
    mAge += dt;

    // 移動
    mPosition.x += mVelocity.x * dt;
    mPosition.y += mVelocity.y * dt;

    computeRectangle();

    // コリジョンチェック

    // 画面外 or 寿命で終了
    const float halfW = mAnimsc->getTexWidth()  * 0.5f;
    const float halfH = mAnimsc->getTexHeight() * 0.5f;
    const int   sw = GetScreenWidth();
    const int   sh = GetScreenHeight();

    Vector2 screenPos 
        = GetWorldToScreen2D(mPosition, static_cast<GamePlay*>(mSequence)->getCamera());;

    const bool out =
        (screenPos.x + halfW < 0) || (screenPos.x - halfW > sw) ||
        (screenPos.y + halfH < 0) || (screenPos.y - halfH > sh);

    if (out || mAge >= mLifeTime) {
        mDead = true;
        setState(Edead);
        // 必要ならここで「消滅演出」や「当たり無効化」を入れる
        // （メモリ破棄はプロジェクトのActor管理方針に合わせて）
    }

    // 当たり判定表示
    //DrawRectangleRec(mRectangle, LIGHTGRAY);
}

void ArrowActor::onStartAttack()
{
    mAnimsc->play(&mAnim); // 矢は回転/ヒラヒラ想定でループ
}

void ArrowActor::computeRectangle()
{
    mRectangle.x = mPosition.x - mAnimsc->getTexWidth()  / 2.0f;
    mRectangle.y = mPosition.y - mAnimsc->getTexHeight() / 2.0f;
    // colRectはmRectangleそのもの
    mAttackInfo.colRect = mRectangle;
}
