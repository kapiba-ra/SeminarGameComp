#include <raylib.h>
#include "Sequence.h"
#include "Title.h"
#include "SoundSystem.h"

const int WindowWidth = 1280;
const int WindowHeight = 720;

void LoadSounds();

int main()
{
	/* Windowの初期化 */
	InitWindow(WindowWidth, WindowHeight, "SeminarProject");
	SetTargetFPS(60); // 60FPS
	SetWindowState(FLAG_VSYNC_HINT); // 垂直同期ON

	// サウンドシステム初期化
	SoundSystem::init();
	// サウンドアセットを一括ロード
	LoadSounds();

	// シーケンスを準備
	Sequence* currentSequence = new Title();

	// メインループ
	while (!WindowShouldClose())
	{
		/* 入力 */
		currentSequence->input();
		
		/* 更新 */
		currentSequence->update();

		/* サウンド更新 */
		SoundSystem::instance().update();

		/* 描画 */
		currentSequence->draw();
		DrawFPS(10, 20); // FPSを表示 消してもいい
	
		// シーケンス切り替え
		Sequence* next = currentSequence->nextSequence();
		if (next != nullptr) {
			currentSequence->unloadData();
			delete currentSequence;
			currentSequence = next;
		}
	}
	
	currentSequence->unloadData();
	delete currentSequence;
	currentSequence = 0;

	SoundSystem::shutdown();

	CloseWindow();
	return 0;
}

// SoundSystem
void LoadSounds()
{
	SoundSystem& s = SoundSystem::instance();
	s.loadBGM("stageBGM", "Assets/uchinari_138.mp3");
	s.loadBGM("BossBGM", "Assets/shippuRaiden.mp3");

	s.loadSE("ItemSE", "Assets/Heal.mp3");
	s.loadSE("ExplosionSE", "Assets/Explosion2.mp3");
	s.loadSE("NormalAtkSE", "Assets/NormalAttack.mp3");
	s.loadSE("ChargeAtkSE", "Assets/ChargeAttack.mp3");
	s.loadSE("DodgeAtkSE", "Assets/DodgeAttack.mp3");
	s.loadSE("GuardSE", "Assets/Guard.mp3");
	s.loadSE("YarareSEa", "Assets/gu.mp3");
	s.loadSE("YarareSEb", "Assets/guaa.mp3");
}
