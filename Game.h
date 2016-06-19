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
    Configure config;//�Q�[���̃��[��
    //vector<Player> players;//�Q���v���C���[�z��
    Players players;//�Q���v���C���[�z��
    
    Table table;//��̏��
    
    //vector<int> mibun;//�g�����
    //vector<int> sekijun;//�ȏ��i��o���Ƀv���C���[�ԍ��������Ă���j
    //vector<int> agari;//��������i�����������Ƀv���C���[�ԍ��������Ă���j
    int turn;//�ǂ̐Ȃ̃^�[�����isekijun�̓Y���j
    
    int convSekiToPID(int idx);//idx�Ԃ̐Ȃɍ����Ă���v���C����ID��Ԃ�
    int whoHave(int suit, int rank);//�N��card[suit][rank]�������Ă��邩
    
    bool isGameEnd();//�Q�[���͏I������H�i���ʂ͌��������j
    bool isBaAlive();
    
    void makeTefuda815(int data[5][8][15]);//UECda�`���̑��M�p�̔z��ɉ��H����
    void makeBafuda815(int data[5][8][15]);//UECda�`���̑��M�p�̔z��ɉ��H����
    
    //void make815Table(int card[8][15]);//�e�[�u���̏����J�[�h�ɏ�������
    //void make815Player(int card[8][15]);//�v���C���[�̏����J�[�h�ɏ�������
    
    void update(const Yaku &yaku);//�ՖʍX�V
    void changeCardsUEC( Result *result );
    void dealCards();
    //bool checkValidCardUEC(Yaku yaku);//��o������o�\���ǂ����m�F���s��
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

