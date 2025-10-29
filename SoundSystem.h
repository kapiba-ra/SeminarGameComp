#pragma once
#include <raylib.h>
#include <string>
#include <unordered_map>

/// <summary>
/// BGM,SEを鳴らすためのクラス 使用方法は下の方に記載
/// </summary>
class SoundSystem {
public:
    static void init() {
        if (!sInstance) sInstance = new SoundSystem();
    }
    static void shutdown() {
        delete sInstance;
        sInstance = nullptr;
    }
    static SoundSystem& instance() { return *sInstance; }

    // BGM再生用
    void update();

    void loadSE(const std::string& name, const std::string& file);
    void loadBGM(const std::string& name, const std::string& file);

    void playBGM(const std::string& name, bool loop = true);
    void playSE(const std::string& name);
    void stopBGM();

private:
    static SoundSystem* sInstance;
    SoundSystem();
    ~SoundSystem();
    Music* mCurrentBGM;
    std::unordered_map<std::string, Music> mBGM;
    std::unordered_map<std::string, Sound> mSE;
    float mBGMVolume;
};

/* 使い方例:
 *   1.ゲーム中に使用するBGM,SEを一括ロード(mainのLoadSoundsでやってます)
 *   第一引数には参照用の名前を渡す
 *   SoundSystem &s = SoundSystem::instance();
 *   s.loadBGM("stageBGM", "Assets/filename");　
 * 　s.loadSE("explosionSE", "Assets/filename");
 *   ...
 *   必要なSE,BGMをロードする
 * 
 *   2.参照用の名前を使って音をならす
 *   SE...s.playSE("explosionSE");
 *   BGM...s.playBGM("stageBGM");
 * 
 *   SoundSystem::update()はmainで呼び出しているのでこれ以上の呼び出しは不要です
 *   Sequenceで呼ぶのも考えましたが,mainの方が問題起こりにくそうなので
 * 
 *   以下蛇足
 *   ・理想はデータ駆動で,JSONファイルに参照用の名前とファイル名をまとめて
 *   読み込むとか出来たら嬉しい
 *   ・Assets/BGM以下のファイルを全読み込みする...とかは出来そうではある
 */