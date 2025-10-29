#pragma once
#include "EnemyActor.h"  
#include <vector>

class Sequence;
class EnemyActor;

/**
 * 敵をスポーンさせる簡易スポナークラス
 * 使い方例:
 *   mSpawner = new EnemySpawner(this);
 *   mSpawner->addSpawnPoint({200.0f, 300.0f});
 *   mSpawner->spawn();                 // ランダムに1体
 *   mSpawner->spawnAt({400.0f, 240.0f}); // 位置指定で1体
 */
class EnemySpawner {
public:
    explicit EnemySpawner(Sequence* seq);

    // 手動スポーン
    EnemyActor* spawn();                      // 登録済み地点 or エリアからランダム
    EnemyActor* spawnAt(const Vector2& pos);  // 指定位置に生成

    // 自動スポーン（必要なら）
    void update(float dt);                    // インターバルに従って自動スポーン
    void setInterval(float seconds) { mInterval = seconds; }
    void setMaxActive(int n)       { mMaxActive = n; }

    // スポーン地点/エリアの登録
    void addSpawnPoint(const Vector2& p) { mSpawnPoints.push_back(p); }
    void clearSpawnPoints() { mSpawnPoints.clear(); }
    void setSpawnArea(const Rectangle& area) { mUseArea = true; mArea = area; }
    void disableSpawnArea() { mUseArea = false; }

private:
    EnemyActor* spawnRandom();
    Vector2 chooseSpawnPosition() const;

private:
    Sequence* mSeq;

    // 自動スポーン用
    float mInterval  = 0.0f;  // 0 なら無効
    float mTimer     = 0.0f;
    int   mMaxActive = 9999;
    
    // 配置
    std::vector<Vector2> mSpawnPoints;
    bool      mUseArea = false;
    Rectangle mArea {0,0,0,0};
};
