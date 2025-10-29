#pragma once
#include <raylib.h>
#include "Actor.h"

class CameraSystem {
public:
    enum class Mode {
        FollowPlayer,   // プレイヤー追従
        Fixed,          // ボス戦
    };

    CameraSystem(float stageWidth);

    void update();
    void setPlayer(Actor* player) { mPlayer = player; }
    void setMode(Mode mode) { mMode = mode; }
    void setFixedTarget(Vector2 target) { mFixedTarget = target; }

    Camera2D& getCamera() { return mCamera; }

private:
    void followPlayer();

    Camera2D mCamera;
    Mode mMode;
    Actor* mPlayer;
    Vector2 mFixedTarget;
    float mStageWidth;
    float mMaxReachedX;
};
