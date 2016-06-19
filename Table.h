#ifndef DEFINE_TABLE
#define DEFINE_TABLE

using namespace std;

#include "Yaku.h"
#include "Player.h"

class Table{
private:
    int64 baBit;//��̏���32bit
	//�E����
    // 0:���
	// 1:�K�i
	// 2:�y�A
	// 3:�P�R
	// 4:�v��
	// 5:����
	// 6:�W���[�J�[�P�R
	
public:
    bool isOnset() const;
    bool isKaidan() const;
    bool isPair() const;
    bool isTanki() const;
    bool isKakumei() const;
    bool isShibari() const;
    bool isJTanki() const;
    bool isChangePhase() const;
    bool is11back() const;
    void setOnset(bool flag);//���t���O���Z�b�g����
    void setKakumei(bool flag);//�v���t���O���Z�b�g����
    void revKakumei();//�v���𔽓]����
    void setKaidan(bool flag);//�K�i�t���O���Z�b�g����
    void setPair(bool flag);//�y�A�t���O���Z�b�g����
    void setTanki(bool flag);//�P�R�t���O���Z�b�g����
    void setShibari(bool flag);//����t���O���Z�b�g����
    void setJTanki(bool flag);//�W���[�J�[�P�R�t���O���Z�b�g����
	int whoseTurn() const;//���݂̃^�[���̐l�̃v���C���[�ԍ���Ԃ�
    
    /*
    //������ւ��mBafuda���Q�Ƃ���̂ł���Ȃ�
    int mNum;
    int mRankR;         //�����N�i�����j
    int mRankL;         //�����N�i�����j
    int mSuits;
    */
    
    int player_id;//�O��̏o�����l��ID
    int preBafuda[8][15];
    Yaku mBafuda;         //��D
    //Player mPlayer;   //�e�v���C���[�̏��
    
    Table();
    
    void firstGame(int cards[8][15]);
    void purge();
    //void setBafuda(int cards[8][15]);
    //void setBaInfo(int cards[8][15]);

    void print() const;
    
    void print2();
};

#endif

