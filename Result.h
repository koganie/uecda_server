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
    //vector< vector< int > > act;//�s���̔z��
    Table mTable;//��o�O�̔Ֆ�
    vector<int> mPA;//pass array
    vector<int> mNA;//num array
    Yaku mYaku;//�s���̖�
    int mId;//�N�̍s����
    
    Action( int id, const Yaku &yaku );
    Action( int id, Table &bTable, vector<int> &passArray, vector<int> &numArray, const Yaku &yaku );
};

class Change{
public:
    int mCards[8][15];
    int mFrom;//�N����
    int mTo;//�N��
    
    Change( int from, int to, const int cards[8][15] );
    
    void print();
    string getStr();
};

class Result{
private:
    
    
    
public:
    vector< vector< vector< int > > > mHands;//�v���C���[�����̍ŏ��̎�D
    vector< Action > mHistory;//�v���C���[�����̍s���̗���
    vector< int > mMibun;//
    vector< int > mAMibun;//
    vector< int > mSekijun;
    vector< int > mAgari;//�オ�������Ƀv���C���[ID�����Ă���
    vector< Change > mChange;//�v���C���[�����̌��������J�[�h
    
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
    vector<string> name;//�e�v���C���[�̖��O
    
    vector<int> tempScore;//���r���_
    vector< vector<int> > score;//�e�v���C���[�̓��_
    vector< vector< vector<int> > > transition;//�e�v���C���[�̐g���̑J��
    
    void push_back(const Result &result);
    void calcScore( int num );
    void calcTransition();
    
    //�����̎��Ԍv���p
    double mStart;
    double mEnd;
    double mTime;
    void clockStart();
    void clockEnd();
    
    //���ʊm�F�p
    void printOneGameReport();
    void print();
    
    //���ʃt�@�C���o�͗p
    void writeScore();
    void writeTransition();
    void writeHistory1();//�b��i�K���j�����o��
    void writeHistory2();//�b��i�K���j�����o��
};
#endif

