#include "Stage.h"
#include <fstream>
#include <string>
#include <vector>
#include <raymath.h>

#include "GamePlay.h"
#include "PlayerActor.h"
#include "EnemySpawner.h"
#include "StageObject.h"
#include "ItemActor.h"

Stage::Stage(GamePlay* sequence)
    : mGamePlay(sequence)
    , mStageHeight(0)
    , mStageWidth(0)
{
    mSpawner = new EnemySpawner(sequence);
}

Stage::~Stage()
{
    delete mSpawner;
    mSpawner = nullptr;
}

void Stage::loadStage(const char* stageName)
{
    // 背景画像を設定
    std::string filename = std::string(stageName) + "tex.png";
    mBG = mGamePlay->getTexture(filename);
    filename = std::string(stageName) + ".txt";

    BossEntrance = Rectangle{0,1,0,0};
    // ' ' : 何もない
    // # : 壁,床
    // w : Breakable Object 障害物
    // E : 敵出現位置
    // B : ボスエリア入口
    // H : 回復アイテム
    std::ifstream file(filename);
    std::string line;
    std::vector<std::vector<char>> tiles;

    // ファイルから二次元ベクトルに読み込む
    while (std::getline(file, line))
    {
        std::vector<char> row;
        for (char c : line)
        {
            row.push_back(c);
        }
        tiles.push_back(row);
    }

    // ステージtxtの文字一つ分の縦横サイズ
    const int tileSize = 40;
    mStageWidth = (int)tiles[0].size() * tileSize;
    mStageHeight = (int)tiles.size() * tileSize;

    for (int y = 0; y < (int)tiles.size(); ++y)
    {
        int startX = -1;
        for (int x = 0; x < (int)tiles[y].size(); ++x)
        {
            // 壁
            if (tiles[y][x] == '#')
            {
                if (startX == -1) startX = x;
            }
            else
            {
                // 床のrectangle作成中の場合
                if (startX != -1)
                {
                    Rectangle r;
                    r.x = (float)startX * tileSize;
                    r.y = (float)y * tileSize;
                    r.width = (float)(x - startX) * tileSize;
                    r.height = (float)tileSize;

                    int tileNum = x - startX;
                    // 床を作成
                    HardObj* obj = new HardObj(mGamePlay, tileNum, r);
                    startX = -1;
                }
                // 敵
                if (tiles[y][x] == 'E')
                {
                    Vector2 pos = { (float)x * tileSize + tileSize / 2.0f, (float)y * tileSize };
                    mEnemySpawnPoints.push_back(pos);
                }
                else if (tiles[y][x] == 'W')
                {
                    Rectangle r;
                    r.x = (float)x * tileSize;
                    r.y = (float)y * tileSize;
                    r.width = (float)tileSize;
                    r.height = (float)tileSize;
                    BreakableObj* bo = new BreakableObj(mGamePlay, r);
                }
                else if (tiles[y][x] == 'B')
                {
                    // ボスエリア
                    BossEntrance.x = (float)x * tileSize;
                    BossEntrance.y = (float)(y - 1) * tileSize;
                    BossEntrance.width = tileSize;
                    BossEntrance.height = tileSize * 2.0f;
                }
                else if (tiles[y][x] == 'H')
                {
                    // TODO: プログラム課題Ex ラスト
                    // ボスを倒してみてください。
                    // 以下の4行をコメントアウトすると、ボス部屋に回復アイテムが設置されます。
                    Actor* actor = new HealingItem(mGamePlay); // クラス名は自分で付けたものに変更してください
                    Vector2 pos = { (float)x * tileSize + tileSize / 2.0f, (float)y * tileSize };
                    actor->setPosition(pos);
                    actor->computeRectangle();
                    // (コメントアウト済み)

                }
            }
        }
        // 右端
        if (startX != -1)
        {
            Rectangle r;
            r.x = (float)startX * tileSize;
            r.y = (float)y * tileSize;
            r.width = (float)(tiles[y].size() - startX) * tileSize;
            r.height = (float)tileSize;

            int tileNum = tiles[y].size() - startX;
            HardObj* obj = new HardObj(mGamePlay, tileNum, r);
            startX = -1;
        }
    }
}

void Stage::update()
{
    PlayerActor* player = mGamePlay->getPlayer();
    Vector2 playerPos = player->getPosition();

    // 敵出現判定
    auto iter = mEnemySpawnPoints.begin();
    while (iter != mEnemySpawnPoints.end()) {
        float dist = Vector2Distance(playerPos, *iter);
        // 敵出現
        if (dist < GetScreenWidth() / 2.0f + 10.0f) {
            mSpawner->spawnAt(*iter);
            iter = mEnemySpawnPoints.erase(iter);
        }
        else {
            ++iter;
        }
    }

    // プレイヤーとボスエリアの接触判定
    if (CheckCollisionRecs(player->getRectangle(), BossEntrance)) {
        mGamePlay->onEnterBossArea();
    }

    DrawTexture(*mBG, 0, 0, WHITE);
}

std::vector<struct Rectangle> Stage::getStageRecs() const
{
    std::vector<struct Rectangle> ret;
    std::vector<StageObject*> objs = mGamePlay->getStageObjs();
    ret.reserve(objs.size());
    for (auto* o : objs) {
        ret.push_back(o->getRectangle());
    }
    return ret;
}
