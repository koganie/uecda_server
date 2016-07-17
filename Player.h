#ifndef DEFINE_PLAYER
#define DEFINE_PLAYER

#include <vector>
#include<netinet/in.h>
#include<sys/socket.h>
//#include "Result.h"
#include "Yaku.h"
#include "common.h"
#include "Random.h"

using namespace std;

class Player{
private:
    
public:
    struct sockaddr_in addr;//�A�h���X
    char name[15];//���O
    int sockfd;
    
    int id;//�v���C���[�ԍ�
    int sekijun;//�ȏ�
    
    int mibun;//�g��
    
    int cards[8][15];//�莝���̃J�[�h
    int cards_num;//�莝���J�[�h�̖���
    vector<Yaku> yakus;//�莝���J�[�h��������S���@��
    
    int change[8][15];//�J�[�h�����ō����o�����J�[�h
    
    void initCard();
    
    void setName(int card[8][15]);
    void setId(int num);
    void subtraction(const int card[8][15]);
    
    bool passed;
    
    Player();
    
    void printCards();
};


class Players{
public:
    vector<Player> id;
    //vector<int> mibun;//�g�����i�j
    vector<int> sekijun;//�ȏ��i��o���Ƀv���C���[�ԍ��������Ă���j
    vector<int> agari;//��������i�����������Ƀv���C���[�ԍ��������Ă���j
    
    int turn;
    int turnId();
    
    //vector<int> sockfd;
    //int sockfd(int id);
    
    int mibunId( int mibun );
    int size();
    void sekigae( Random *random );
    void reset();
    bool isInequality();
    //void update( Result &result );
    
    void setCardsNum();
    void nextTurn();
    void recover();//�p�X���畜�A������
    void addAgari( int num );
    //bool pass();//�p�X���Ď��̐l�ɉ�
    
    void print();
    int convIDtoSekiNum(int num);
};


#endif

