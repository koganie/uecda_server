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
    Configure config;//�Q�[���̃��[��
    Random *random;//����������
    
    int turn;//�ǂ̐Ȃ̃^�[�����isekijun�̓Y���j
    
    int convSekiToPID(int idx);//idx�Ԃ̐Ȃɍ����Ă���v���C����ID��Ԃ�
    int whoHave(int suit, int rank);//�N��card[suit][rank]�������Ă��邩
    
    bool isGameEnd();//�Q�[���͏I������H�i���ʂ͌��������j
    bool isBaAlive();
    
    void makeTefuda815(int data[5][8][15]);//UECda�`���̑��M�p�̔z��ɉ��H����
    void makeBafuda815(int data[5][8][15]);//UECda�`���̑��M�p�̔z��ɉ��H����
    
    void changeCardsUEC( Result *result );
    void exploit( Result *result );
    
    bool conv815toYaku(Yaku *yaku, int card[8][15]);
    bool isSubmittableYaku(const Yaku &yaku);
    
    void setMibun();
    void setSekijun();
    void makePassArray( vector<int> *array );
    void makeNumArray( vector<int> *array );
    
public:
    Players players;//�Q���v���C���[�z��
    Table table;//��̏��

    Game(const Configure &config, const Players &p, Result *result, Random *r);
    
    void dealCards( Result *result );
    void dealCards( Result *result , vector<string> &tefudaSet);
    void sendFirstCards( Result *result );
    
    void start( Result *result );

    void update(const Yaku &yaku);//�ՖʍX�V
    void purge();//��𗬂�
    void pass();//���݃v���C�����p�X������
    void recover();//�p�X���畜������
    void nextTurn();//���̃v���C���[�Ƀ^�[������
};

bool checkChangeCardUEC(int data[8][15], Player &player);

#endif

