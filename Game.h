#ifndef DEFINE_GAME
#define DEFINE_GAME

#include <vector>
#include "Random.h"
#include "Configure.h"
#include "Table.h"
#include "Player.h"
#include "Result.h"

using namespace std;

class Game{
private:
    Configure config;//ゲームのルール
    Random *random;//乱数生成器
    
    int turn;//どの席のターンか（sekijunの添字）
    
    int convSekiToPID(int idx);//idx番の席に座っているプレイヤのIDを返す
    int whoHave(int suit, int rank);//誰がcard[suit][rank]を持っているか
    
    bool isGameEnd();//ゲームは終わった？（順位は決したか）
    bool isBaAlive();
    
    void makeTefuda815(int data[5][8][15]);//UECda形式の送信用の配列に加工する
    void makeBafuda815(int data[5][8][15]);//UECda形式の送信用の配列に加工する
    
    void changeCardsUEC( Result *result );
    void exploit( Result *result );
    
    bool conv815toYaku(Yaku *yaku, int card[8][15]);
    bool isSubmittableYaku(const Yaku &yaku);
    
    void setMibun();
    void setSekijun();
    void makePassArray( vector<int> *array );
    void makeNumArray( vector<int> *array );
    
public:
    Players players;//参加プレイヤー配列
    Table table;//場の情報

    Game(const Configure &config, const Players &p, Result *result, Random *r);
    
    void dealCards( Result *result );
    void dealCards( Result *result , vector<string> &tefudaSet);
    void sendFirstCards( Result *result );
    
    void start( Result *result );

    void update(const Yaku &yaku);//盤面更新
    void purge();//場を流す
    void pass();//現在プレイヤがパスをした
    void recover();//パスから復活する
    void nextTurn();//次のプレイヤーにターンを回す
};

bool checkChangeCardUEC(int data[8][15], Player &player);

#endif

