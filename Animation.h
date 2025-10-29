#pragma once
#include <vector>
#include <raylib.h>

/// <summary>
/// アニメーション情報
/// </summary>
struct Animation
{
    std::vector<Texture2D*> frames;    // 各フレームのテクスチャ
    float fps = 24.0f;                 // 再生FPS
    bool loop;                         // ループアニメーションかどうか
};
