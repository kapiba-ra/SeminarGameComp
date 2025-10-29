#pragma once
#include <vector>
#include <raylib.h>

class Stage
{
public:
    Stage(class GamePlay* sequence);
    ~Stage();
	void loadStage(const char* stageName);
    void update();

    int getStageWidth() const { return mStageWidth; }

    std::vector<struct Rectangle> getStageRecs() const;

private:
    class GamePlay* mGamePlay;
    class EnemySpawner* mSpawner;

    int mStageWidth;
    int mStageHeight;
    struct Rectangle BossEntrance{0,0,0,0};
    std::vector<Vector2> mEnemySpawnPoints;
    Texture2D* mBG; // ステージの背景
};

