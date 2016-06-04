#ifndef DEFINE_RESULT
#define DEFINE_RESULT

#include <iostream>
#include <vector>
#include "Yaku.h"
#include "Table.h"
#include "Player.h"
#include <ctime>
#include <cstring>
#include <fstream>

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
};

class Result{
private:
    vector< Action > mHistory;//�v���C���[�����̍s���̗���
    
    vector< vector< vector< int > > > mHands;//�v���C���[�����̍ŏ��̎�D
    
public:
    vector< int > mMibun;//
    vector< int > mAMibun;//
    vector< int > mAgari;//�オ�������Ƀv���C���[ID�����Ă���
    vector< Change > mChange;//�v���C���[�����̌��������J�[�h
    
    void addAct( int id, const Yaku &yaku );
    void addAct( int id, Table &bTable, vector<int> &passArray, vector<int> &numArray, const Yaku &yaku );
    void addChange( int from, int to, const int cards[8][15] );
    void addAgari( int id );
    void addHands( const int cards[8][15]);
    void setMibun( vector<Player> *players );
    void setup( vector<Player> *players);
    void setup( Players *players);
    void finish(Players &players);
    void update( Players *players );
    
    bool sekigae;
    bool reset;
    
    
    void print();
};

class Results{
private:
    vector<Result> results;
    
    
public:
    Results( Players &players );
    vector<string> name;//�e�v���C���[�̖��O
    
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
};
#endif

