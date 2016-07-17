#ifndef DEFINE_RESULT
#define DEFINE_RESULT

#include <iostream>
#include <vector>
#include "Yaku.h"
#include "Table.h"
#include "Player.h"
#include "Configure.h"
#include <ctime>
#include <cstring>
#include <fstream>
#include <sstream>

using namespace std;

class Action{
public:
    //vector< vector< int > > act;//行動の配列
    Table mTable;//提出前の盤面
    vector<int> mPA;//pass array
    vector<int> mNA;//num array
    Yaku mYaku;//行動の役
    int mId;//誰の行動か
    
    Action( int id, const Yaku &yaku );
    Action( int id, Table &bTable, vector<int> &passArray, vector<int> &numArray, const Yaku &yaku );
};

class Change{
public:
    int mCards[8][15];
    int mFrom;//誰から
    int mTo;//誰に
    
    Change( int from, int to, const int cards[8][15] );
    
    void print();
    string getStr();
};

class Result{
private:
    
    
    
public:
    vector< vector< vector< int > > > mHands;//プレイヤーたちの最初の手札
    vector< Action > mHistory;//プレイヤーたちの行動の履歴
    vector< int > mMibun;//
    vector< int > mAMibun;//
    vector< int > mSekijun;
    vector< int > mAgari;//上がった順にプレイヤーIDを入れていく
    vector< Change > mChange;//プレイヤーたちの交換したカード
    
    void addAct( int id, const Yaku &yaku );
    void addAct( int id, Table &bTable, vector<int> &passArray, vector<int> &numArray, const Yaku &yaku );
    void addChange( int from, int to, const int cards[8][15] );
    void addAgari( int id );
    void addHands( const int cards[8][15]);
    void setMibun( vector<Player> *players );
    //void setup( vector<Player> *players);
    void setup( Players *players, Configure &config);
    void finish(Players &players);
    void update( Players *players );
    
    void setFirstCards( int cards[5][8][15] );
    
    Result();
    bool sekigae;
    bool reset;
    Configure mConfig;
    
    void print();
};

class Results{
private:
    vector<Result> results;
    
public:
    Results( Players &players );
    vector<string> name;//各プレイヤーの名前
    
    vector<int> tempScore;//中途得点
    vector< vector<int> > score;//各プレイヤーの得点
    vector< vector< vector<int> > > transition;//各プレイヤーの身分の遷移
    
    void push_back(const Result &result);
    void calcScore( int num );
    void calcTransition();
    
    //試合の時間計測用
    double mStart;
    double mEnd;
    double mTime;
    void clockStart();
    void clockEnd();
    
    //結果確認用
    void printOneGameReport();
    void print();
    
    //結果ファイル出力用
    void writeScore();
    void writeTransition();
    void writeHistory1();//暫定（適当）棋譜出力
    void writeHistory2();//暫定（適当）棋譜出力
};
#endif

