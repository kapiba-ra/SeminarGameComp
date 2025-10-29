#pragma once
#include "Actor.h"

/// <summary>
/// ステージを構成するオブジェクトの基底クラス
/// </summary>
class StageObject :
    public Actor
{
public:
    enum Type
    {
        Ehard,
        Ebreakable
    };
    StageObject(class Sequence* sequence, Type type);
    virtual ~StageObject();

    class HpComponent* getHpComp() { return mHpComp; }
    Type getType() const { return mType; }

protected:
    Type mType;
    class SpriteComponent* mSpriteComp;
    class HpComponent* mHpComp;
};

/// <summary>
/// 床とか壁
/// 横方向で一つのActorとしてまとめている
/// </summary>
class HardObj :
    public StageObject
{
public:
    HardObj(class Sequence* sequence, int tileNum, Rectangle r);
    ~HardObj() override;

private:
    const int mTileNum;    // (横長の)床を構成するタイルの数
    Texture2D mTexture;    // シーケンスに持たせてもいいが,作って登録する手間は省く
};

/// <summary>
/// 破壊可能オブジェクト(たる)
/// </summary>
class BreakableObj :
    public StageObject
{
public:
    BreakableObj(class Sequence* sequence, Rectangle r);
    void update() override;
};