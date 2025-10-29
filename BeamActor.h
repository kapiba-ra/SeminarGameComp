#pragma once
#include "Actor.h"
#include "raylib.h"

class GamePlay;

class BeamActor : public Actor {
public:
    BeamActor(GamePlay* gp, Vector2 start, int forward);
    ~BeamActor();

    void update() override;
    void draw() const; // baseがvirtualでない前提

    // FIX: 基底と同シグネチャ（const参照返し）＋ override を付与
    inline const Rectangle& getRectangle() const override { return mRectangle; } // FIX

    inline int  getForward() const { return mForward; }
    inline void setForward(int fwd) { mForward = (fwd >= 0) ? 1 : -1; }

    inline void setLife(float sec) { mLife = sec; }
    inline void setBeamSize(float w, float h) { mBeamW = w; mBeamH = h; }

private:
    GamePlay* mGP{nullptr};

    // 判定（ビームAABB：左端中央基準）
    Rectangle mRectangle{};
    float     mBeamW{900.0f};
    float     mBeamH{24.0f};
    float     mLife{0.25f};
    int       mForward{1};

    // 見た目（判定に使わない）
    Texture2D* mArrowTex{nullptr};
    bool       mArrowTexValid{false};
    float      mT{0.0f};
    float      mVisSpeed{4.0f}; // 0→1 の増分/秒
    Vector2    mVisPos{0, 0};

    // DEBUG: 全インスタンスで共有する可視化トグル
    static bool sDebugBeam;        // DEBUG
    bool        mHitThisFrame{false}; // DEBUG: 当たり中の色分け

    // FIX: override を明示
    void computeRectangle() override; // FIX
    void updateDamage();
    void updateVisualArrow(float dt);
};
