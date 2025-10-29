#include "EnemySpawner.h"
#include "Sequence.h"

#include "WeakEnemy.h"

EnemySpawner::EnemySpawner(Sequence* seq)
    : mSeq(seq)
{
}

void EnemySpawner::update(float dt)
{
    if (mInterval <= 0.0f) return;  // 自動スポーン無効
    mTimer += dt;
    if (mTimer >= mInterval) {
        spawnRandom();
        mTimer = 0.0f;
    }
}

EnemyActor* EnemySpawner::spawn()
{
    return spawnRandom();
}

EnemyActor* EnemySpawner::spawnAt(const Vector2& pos)
{
    // 通常の敵を生成
    EnemyActor* enemy;
    if (GetRandomValue(0, 1)) {
        enemy = new MeleeEnemy(mSeq);
    }
    else {
        enemy = new RangedEnemy(mSeq);
    }

    // 位置設定と当たり判定更新（既存のAPI前提）
    enemy->setPosition(pos);
    enemy->computeRectangle();

    return enemy;
}

EnemyActor* EnemySpawner::spawnRandom()
{
    Vector2 pos = chooseSpawnPosition();
    return spawnAt(pos);
}

Vector2 EnemySpawner::chooseSpawnPosition() const
{
    // 1) スポーンエリアが設定されていればそこから
    if (mUseArea && mArea.width > 0 && mArea.height > 0) {
        Vector2 p;
        p.x = mArea.x + static_cast<float>(GetRandomValue(0, (int)mArea.width));
        p.y = mArea.y + static_cast<float>(GetRandomValue(0, (int)mArea.height));
        return p;
    }

    // 2) スポーン地点群からランダム
    if (!mSpawnPoints.empty()) {
        int idx = GetRandomValue(0, static_cast<int>(mSpawnPoints.size()) - 1);
        return mSpawnPoints[static_cast<size_t>(idx)];
    }

    // 3) 何も設定されていなければデフォルト
    return { 100.0f, 100.0f };
}
