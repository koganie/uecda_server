#ifndef DEFINE_GAME
#define DEFINE_GAME

#include <vector>
#include "Configure.h"
#include "Table.h"
#include "Player.h"
#include "Result.h"

using namespace std;

class Game{
private:
    Configure config;//ゲームのルール
    //vector<Player> players;//参加プレイヤー配列
    Players players;//参加プレイヤー配列
    
    Table table;//場の情報
    
    //vector<int> mibun;//身分情報
    //vector<int> sekijun;//席情報（提出順にプレイヤー番号が入っている）
    //vector<int> agari;//あがり情報（あがった順にプレイヤー番号が入っている）
    int turn;//どの席のターンか（sekijunの添字）
    
    int convSekiToPID(int idx);//idx番の席に座っているプレイヤのIDを返す
    int whoHave(int suit, int rank);//誰がcard[suit][rank]を持っているか
    
    bool isGameEnd();//ゲームは終わった？（順位は決したか）
    bool isBaAlive();
    
    void makeTefuda815(int data[5][8][15]);//UECda形式の送信用の配列に加工する
    void makeBafuda815(int data[5][8][15]);//UECda形式の送信用の配列に加工する
    
    //void make815Table(int card[8][15]);//テーブルの情報をカードに書き込む
    //void make815Player(int card[8][15]);//プレイヤーの情報をカードに書き込む
    
    void update(const Yaku &yaku);//盤面更新
    void changeCardsUEC( Result *result );
    void dealCards();
    //bool checkValidCardUEC(Yaku yaku);//提出役が提出可能かどうか確認を行う
    bool conv815toYaku(Yaku *yaku, int card[8][15]);
    bool isSubmittableYaku(const Yaku &yaku);
    //bool isCardChangePhase();
    
    //
    void purge();
    //
    void pass();
    void recover();
    void nextTurn();
    
    void setMibun();
    void setSekijun();
    void makePassArray( vector<int> *array );
    void makeNumArray( vector<int> *array );
    
public:
    //Game(const Configure &config, const vector<Player> &p, Result *result);
    Game(const Configure &config, const Players &p, Result *result);
    
    void start( Result *result );
    //void sekigae();
    //void reset();
    
};

bool checkChangeCardUEC(int data[8][15], Player &player);

#endif

