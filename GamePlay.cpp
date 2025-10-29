#include "GamePlay.h"
#include "GameClear.h"
#include "GameOver.h"

#include <raylib.h>
#include <cmath>
#include <fstream>
#include <string>
#include <algorithm>
#include <iostream>
#include <cfloat>

// Actor
#include "PlayerActor.h"
#include "EnemyActor.h"
#include "WeaponActor.h"
#include "Boss.h"
#include "StageObject.h"
#include "BossActor.h"
// Component
#include "PlayerMove.h"
#include "EnemyMove.h"
#include "SpriteComponent.h"
#include "HpComponent.h"
// system
#include "Stage.h"
#include "CameraSystem.h"
#include "UIScreen.h"
#include "HUD.h"
#include "DamageUI.h"
#include "BossHUD.h"
#include "SoundSystem.h"

GamePlay::GamePlay()
{
    mStage = new Stage(this);
    // ステージファイルの読み込み
    mStage->loadStage("Assets/stage0");

    // とりあえずプレイヤーとスポナーを生成
    mPlayer = new PlayerActor(this);

    // カメラシステムの初期化
    mCameraSystem = new CameraSystem((float)mStage->getStageWidth());
    mCameraSystem->setPlayer(mPlayer);
    mCameraSystem->setMode(CameraSystem::Mode::FollowPlayer);
    mHUD = new HUD(this);
    mDamageUI = new DamageUI(this);
    
    // BGMを鳴らす
    SoundSystem::instance().playBGM("stageBGM");
}

GamePlay::~GamePlay()
{
    // deleteはSequenceやdestroyEnemyで対応するのでここでは削除しない
    /*while (!mEnemies.empty()) {
        delete mEnemies.back();
    }
    delete mPlayer;*/
    delete mHUD;
    mHUD = nullptr;

    delete mCameraSystem;
    mCameraSystem = nullptr;
}

void GamePlay::input()
{
    // テスト入力
    if (IsKeyPressed(KEY_ENTER)) {
        mNext = new GameClear();
    }
    else if (IsKeyPressed(KEY_RIGHT_SHIFT)) {
        mNext = new GameOver();
    }
    // すべてのActorのinputを呼ぶ
    mUpdatingActors = true;
    for (Actor* actor : mActors) {
        actor->input();
    }
    mUpdatingActors = false;
}

void GamePlay::update()
{
    /* ここでdrawに関する関数を呼ぶのはきれいではないかも */
    /* デバッグ用にupdate内でもカメラを考慮してdrawしたいのでここに置いた */
    BeginDrawing();
    ClearBackground(SKYBLUE);
    //// カメラに従って描画（ゲーム画面）
    BeginMode2D(mCameraSystem->getCamera());


    mCameraSystem->update();

    // Actorのupdate
    mUpdatingActors = true;
    for (Actor* actor : mActors) {
        actor->update();
    }
    mUpdatingActors = false;

    // 保留中のActorを追加
    for (auto pending : mPendingActors)
    {
        mActors.emplace_back(pending);
    }
    mPendingActors.clear();

    // Dead状態のActorをdelete
    for (auto actor : mActors)
    {
        if (actor->getState() == Actor::Edead)
        {
            delete actor;
        }
    }

    // ui update
    for (auto ui : mUIStack) {
        if (ui->GetState() == UIScreen::EActive) {
            ui->update();
        }
    }
    auto iter = mUIStack.begin();
    while (iter != mUIStack.end()) {
        if ((*iter)->GetState() == UIScreen::EClosing) {
            delete* iter;
            iter = mUIStack.erase(iter);
        }
        else {
            ++iter;
        }
    }

    mStage->update();

    // playerが死んだら次のシーケンスを設定(とりあえずここに書く)
    if (mPlayer) {
        if (mPlayer->isDead()) {
            SoundSystem::instance().stopBGM();
            mNext = mNext = new GameOver();
        }
    }
    checkCheatCode();

}

void GamePlay::draw()
{
    //mStage->draw();

    for (auto sprite : mSprites)
    {
        sprite->draw();
    }
    // カメラ考慮の描画を終える
    EndMode2D();

    // uiの描画
    DrawText("GamePlay", 100, 100, 40, BLACK);
    DrawText("Press ENTER -> GameClear", 100, 200, 20, GRAY);
    DrawText("Press RightShift -> GameOver", 100, 300, 20, GRAY);
    for (auto ui : mUIStack) {
        ui->draw();
    }

    EndDrawing();
}

void GamePlay::unloadData()
{
    Sequence::unloadData();
    while (!mActors.empty())
    {
        delete mActors.back();
    }
}

Sequence* GamePlay::nextSequence()
{
    return mNext;
}

void GamePlay::addEnemy(EnemyActor* enemy)
{
    mEnemies.emplace_back(enemy);
}

void GamePlay::removeEnemy(EnemyActor* enemy)
{
    // mEnemiesから削除
    auto iter = std::find(mEnemies.begin(), mEnemies.end(), enemy);
    if (iter != mEnemies.end()) {
        std::iter_swap(iter, mEnemies.end() - 1);
        mEnemies.pop_back();
    }
}

void GamePlay::addStageObj(StageObject* enemy)
{
    mObjects.emplace_back(enemy);
}

void GamePlay::removeStageObj(StageObject* enemy)
{
    auto iter = std::find(mObjects.begin(), mObjects.end(), enemy);
    if (iter != mObjects.end()) {
        std::iter_swap(iter, mObjects.end() - 1);
        mObjects.pop_back();
    }
}

Camera2D GamePlay::getCamera() const
{
    if (mCameraSystem) return mCameraSystem->getCamera();
    else return Camera2D();
}

void GamePlay::addSprite(SpriteComponent* sprite)
{
    // スプライトの描画順序を考慮して追加
    int myDrawOrder = sprite->getDrawOrder();
    auto iter = mSprites.begin();
    for (;
        iter != mSprites.end();
        ++iter)
    {
        if (myDrawOrder < (*iter)->getDrawOrder())
        {
            break;
        }
    }
    // 適切な位置に挿入
    mSprites.insert(iter, sprite);
}

void GamePlay::removeSprite(SpriteComponent* sprite)
{
    auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
    mSprites.erase(iter);
}

void GamePlay::onEnterBossArea()
{
    SoundSystem::instance().playBGM("BossBGM");

    // 敵全削除
    while (!mEnemies.empty()) {
        delete mEnemies.back();
    }
    // ステージオブジェクト全削除
    while (!mObjects.empty()) {
        delete mObjects.back();
    }

    mStage->loadStage("Assets/stage0_boss");
    // ステージから位置を拾ってきてもいいかも
    mPlayer->setPosition(Vector2{ 120.0f, 400.0f });
    mPlayer->computeRectangle();
    mCameraSystem->setMode(CameraSystem::Mode::Fixed);
    
    
    // ★ ボス出現：ボス部屋ロード後に生成する（ロードで敵を全消しするため
    auto* boss = new Boss(this);
    boss->setPosition(Vector2{
        (float)mStage->getStageWidth() * 0.70f,
        0.0f // ← いったん0で置いて、後で床にスナップ
    });
    boss->computeRectangle();

    
    // ★ BossHUD を生成してボスを紐づけ
    if (mBossHUD) { /* 古いのがあれば閉じる */ mBossHUD->Close(); }
    mBossHUD = new BossHUD(this);
    mBossHUD->setBoss(boss);
// ★ 床スナップ：ボスの足元直下の床トップに位置合わせ
    {
         const float bossW = boss->getRectangle().width;
         const float bossH = boss->getRectangle().height;
         const float bossX = boss->getPosition().x;

        // X方向に少し幅を持たせて、真下の床を検索
        const float probeHalfW = bossW * 0.35f;
        const float probeLeft  = bossX - probeHalfW;
        const float probeRight = bossX + probeHalfW;

        float bestY = FLT_MAX; // 見つけた床の「上端」の最小値（＝一番近い床）
        for (const auto& r : mStage->getStageRecs()) {
            // X方向が重なっていて、自分より下側（上端が0より大きい想定）
            const bool xOverlap = !(r.x + r.width < probeLeft || probeRight < r.x);
            if (!xOverlap) continue;
            // r は床ブロック：その上端に立ちたい
            if (r.y < bestY) bestY = r.y;
        }
        if (bestY < FLT_MAX) {
            // 俯瞰系：矩形は左上基準、ボスの位置は矩形中央基準なら調整
            // 本プロジェクトのActorは mPosition を矩形中心としている実装が多いので、
            // 「床の上端 ＝ bestY」にボスの矩形の“底”を合わせる
            const float newY = bestY - (bossH * 0.5f);
            boss->setPosition(Vector2{ bossX, newY });
            boss->computeRectangle();
        }
    }

}

void GamePlay::onBossKilled()
{
    mBossHUD->Close();
    mNext = new GameClear();
}

void GamePlay::checkCheatCode()
{
    int ch;
    while ((ch = GetCharPressed()) != 0) {
        if (ch >= 32 && ch <= 126) {
            char c = static_cast<char>(ch);
            if (c >= 'A' && c <= 'Z') c = char(c - 'A' + 'a');
            mCheatBuf.push_back(c);
            if (mCheatBuf.size() > 16) {
                mCheatBuf.erase(0, mCheatBuf.size() - 16);
            }
        }
    }
    if (!mCheatBuf.empty()) {
        if (mCheatBuf.find("fuwa") != std::string::npos) {
            mCheatBuf.clear();
            onEnterBossArea();
        }
    }
}
