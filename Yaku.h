#ifndef DEFINE_YAKU
#define DEFINE_YAKU

#include "bit.h"

using namespace std;

class Yaku{
private:
    int64 mCardBit;       //�J�[�h�̃r�b�g�\��
    int mCards[8][15];
    
public:
    int mNum;           //����
    int mRankR;         //�����N�i�����j
    int mRankL;         //�����N�i�����j
    
    int mSuits;         //�X�[�g
    int mJposSuit;
    int mJposRank;
    bool mSpe3;
    
    bool isKaidan() const;//�K�i�Ȃ�True�A�ȉ����l��
    bool isPair() const;
    bool isTanki() const;
    bool isJTanki() const;
    bool isJUsed() const;
    bool isKakumei() const;
    //bool isShibari() const;
    bool is8giri() const;
    bool isSpade3() const;
    bool isPass() const;
    
    void setSuit(int s);
    void setTanki();
    void setPair();
    void setKaidan();
    void setJTanki();
    
    int64 getCardBit() const;//�J�[�h�r�b�g��Ԃ�
    void printBit() const;
    void printBit2() const;
    void printBit3() const;
    
    void print() const;
	
	Yaku();
    
    //�R���X�g���N�^
    //������J�[�h�r�b�g�A�X�[�g�A�����A�����̃����N�A�E���̃����N�i�K�i�Ajoker�P�R�̂Ƃ��ȊO�͓��������j�A�g��joker�̃X�[�g�A�g��joker�̃����N�i�g��Ȃ��Ƃ���-1�j
    Yaku(int64 cd, int suits, int num, int rank, int rank2, int jps, int jpr);
    //joker�g��Ȃ��R���X�g���N�^��p�ӂ��Ă�����
    
    //��������
    void clear();
    void init();
    void demoPass();
    
    //�Ⴂ���悭�킩���̂ŉ�������
    void setBit(int src[8][15]);//[8][15]�z�񂩂�����i�[���� //��D�̉�͗p�ł���
    void set815ToBit(int src[8][15]);//815�z���bit�ɕϊ�����
    void setBitTo815(int dest[8][15]) const;//bit��815�z��ɕϊ�����
};

#endif

