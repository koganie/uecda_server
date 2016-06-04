#ifndef DEFINE_CONFIGURE
#define DEFINE_CONFIGURE

#include<string>

using namespace std;

class Configure{
public:
    int PROTOCOL; //通信プロトコル番号
    
    //ゲームに関する設定
    bool RULE_KAKUMEI;//「革命」を使うか
    bool RULE_SHIBARI;//「縛り」を使うか
    bool RULE_KAIDAN;//「階段」を使うか
    bool RULE_8GIRI;//「8切り」を使うか
    bool RULE_11BACK;//「11バック」を使うか TODO
    bool RULE_MIYAKOOCHI;//「都落ち」を使うか TODO
    
    int RULE_SEKIGAE_NUM;//何試合行うと席順をリセットするか
    int RULE_RESET_NUM;//何試合行うと身分等のリセットを行うか
    
    int GAME_NUM;//何回のゲームを行うか
    int PLAYER_NUM;//プレイヤーの数
    
    int POINT;//得点の加算方式
    
    int JOKER_NUM;//ジョーカーの枚数 TODO
    
    //通信に関する設定
    int PORT_NUM;//ポート番号
    
    //設定ファイル
    char cfg_file[100];
    
    Configure();
    Configure(int argc, char* argv[]);//引数を解読するこんすとらくた
    bool setRule();//cfg_fileを読み込んでルールを確定する
    
    bool isSekigae(int game_count);//席替えが発生するか
    bool isReset(int game_count);//リセットが発生するか
    
    void printRules();
};

#endif

