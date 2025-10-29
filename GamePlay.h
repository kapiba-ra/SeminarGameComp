#pragma once
#include <string>
#include "Sequence.h"

/// <summary>
/// GamePlayシーケンス
/// </summary>
class GamePlay :
    public Sequence
{
public:
    GamePlay();
    ~GamePlay() override;

    void input() override;
    void update() override;
    void draw() override;
    void unloadData() override;
    Sequence* nextSequence() override;

    void addEnemy(class EnemyActor* enemy);
    void removeEnemy(class EnemyActor* enemy);
    void addStageObj(class StageObject* enemy);
    void removeStageObj(class StageObject* enemy);

    class Stage* getStage() const { return mStage; }
    std::vector<class EnemyActor*> const getEnemies() { return mEnemies; }
    std::vector<class StageObject*> const getStageObjs() { return mObjects; }
    // 敵やその他がプレイヤー情報を取得するために定義
    class PlayerActor* getPlayer() const { return mPlayer; }
    struct Camera2D getCamera() const;
    class DamageUI* getDamageUI() const { return mDamageUI; }

    void addSprite(class SpriteComponent* sprite);
    void removeSprite(class SpriteComponent* sprite);

    void onEnterBossArea();
    void onBossKilled();

private:
    Sequence* mNext = nullptr;
    class Stage* mStage;
    // 敵の配列(主にCollision用に使う)
    std::vector<class EnemyActor*> mEnemies;
    // ステージオブジェクトの配列(Collision)
    std::vector<class StageObject*> mObjects;
    class PlayerActor* mPlayer;
    // Spriteの配列
    std::vector<class SpriteComponent*> mSprites;
    class CameraSystem* mCameraSystem;
    class HUD* mHUD;
    class DamageUI* mDamageUI;
    class BossHUD* mBossHUD; // ★ 追加：ボス用HUD

    std::string mCheatBuf;
    void checkCheatCode();
};

// collisionの処理はcollisionManagerみたいなの作って
// 外に出してもいいかもです
