#include "Table.h"
#include <iostream>

using namespace std;

Table::Table(){
    baBit = 0ull;
    /*
    mNum = 0;
    mRankR = 0;
    mRankL = 0;
    mSuits = 0;
    */
    setOnset(1);//���ɃZ�b�g
    mBafuda.init();//�J�[�h�͍폜
}

bool Table::isOnset() const{
    return (baBit&(IS_BA_ONSET))!=0;//��ꂩ�H
}
bool Table::isKaidan() const{
    return (baBit&(IS_BA_KAIDAN))!=0;//�K�i���H
}
bool Table::isPair() const{
    return (baBit&(IS_BA_PAIR))!=0;//�y�A���H
}
bool Table::isTanki() const{
    //return (baBit&(IS_BA_TANKI))!=0;//�P�R���H
    return mBafuda.isTanki();
}
bool Table::isKakumei() const{
    return (baBit&(IS_BA_KAKUMEI))!=0;//�v�����H
}
bool Table::isShibari() const{
    return (baBit&(IS_BA_SHIBARI))!=0;//���肩�H
}
bool Table::isJTanki() const{
    return (baBit&(IS_BA_JOKER))!=0;//�W���[�J�[�P�R���H
}
bool Table::is11back() const{
    return (baBit&(IS_BA_11BACK))!=0;//11�o�b�N���H
}
bool Table::isChangePhase() const{
    return (baBit&(IS_BA_CHANGE_PHASE))!=0;//�J�[�h�����̃^�C�~���O���H
}
void Table::setOnset(bool flag){//���t���O���Z�b�g����
    if(flag)    baBit |=  IS_BA_ONSET;
    else        baBit &= ~IS_BA_ONSET;
}
void Table::setKakumei(bool flag){//�v���t���O���Z�b�g����
    if(flag)    baBit |=  IS_BA_KAKUMEI;
    else        baBit &= ~IS_BA_KAKUMEI;
}
void Table::revKakumei(){//�v���𔽓]����
    if(isKakumei()) setKakumei(0);//�v����Ԃł���Ȃ�߂�
    else            setKakumei(1);//�ʏ��Ԃł���Ȃ痧�Ă�
}
void Table::setKaidan(bool flag){//�K�i�t���O���Z�b�g����
    if(flag)    baBit |=  IS_BA_KAIDAN;
    else        baBit &= ~IS_BA_KAIDAN;
}
void Table::setPair(bool flag){//�y�A�t���O���Z�b�g����
    if(flag)    baBit |=  IS_BA_PAIR;
    else        baBit &= ~IS_BA_PAIR;
}
void Table::setTanki(bool flag){//�P�R�t���O���Z�b�g����
    if(flag)    baBit |=  IS_BA_TANKI;
    else        baBit &= ~IS_BA_TANKI;
}
void Table::setShibari(bool flag){//����t���O���Z�b�g����
    if(flag)    baBit |=  IS_BA_SHIBARI;
    else        baBit &= ~IS_BA_SHIBARI;
}
void Table::setJTanki(bool flag){//�W���[�J�[�P�R�t���O���Z�b�g����
    if(flag)    baBit |=  IS_BA_JOKER;
    else        baBit &= ~IS_BA_JOKER;
}

/*
int Table::whoseTurn() const{//���݂̃^�[���̐l�̃v���C���[�ԍ���Ԃ�
    return mPlayers.turn;
}
*/

void Table::firstGame(int cards[8][15]){//�Q�[���̏�����
    baBit = 0;
    /*
    mNum = 0;
    mRankR = 0;
    mRankL = 0;
    mSuits = 0;
    */
    setOnset(1);//���ɃZ�b�g
    mBafuda.init();//�J�[�h�͍폜
    //mPlayers.set(cards);//�J�[�h��5�s�ڈȍ~�̏��ŃZ�b�g
}

void Table::purge(){//��𗬂��i�p��\�����킩��Ȃ������j
    baBit &= (IS_BA_KAKUMEI);   //�v����Ԃ͏����邱�Ƃ͂Ȃ�
    baBit |= (IS_BA_ONSET);     //���ɂ���
    /*
    mNum = 0;
    mRankR = 0;
    mRankL = 0;
    mSuits = 0;
    */
    mBafuda.clear();      //�J�[�h������
    //mPlayers.recover(); //�v���C���[�̃p�X��Ԃ���������
}

/*
void Table::setBafuda(int cards[8][15]){//��D��ۑ��i��D����ǂݎ�����̂����X�V����j
    mBafuda.clear();
    mBafuda.setBit(cards);
    
    if( mBafuda.isKakumei() ) revKakumei();//�J�[�h���v���𔭐�������Ȃ��Ԃ𔽓]����
    setKaidan( mBafuda.isKaidan() );      //�K�i
    setPair( mBafuda.isPair() );          //�y�A
    
    mNum = mBafuda.mNum;                  //����
    if(mNum==1){//�J�[�h����            //�P�R
        setTanki( 1 );
        //setJoker( mBafuda.isJoker() );    //joker�P�R
        setJTanki( mBafuda.isJTanki() );    //joker�P�R
    }else if(mNum==0){//�����炭�����͂߂����ɂ͂���Ȃ�
        setOnset( 1 );
    }
    /*
    if( isKakumei() ){                  //�����A�v���Ȃ獶�[���A�ʏ�Ȃ�E�[��
        mRank = mBafuda.mRankL;//���[
    }else{
        mRank = mBafuda.mRankR;//�E�[
    }
    
    mRankL = mBafuda.mRankL;//���[
    mRankR = mBafuda.mRankR;//�E�[
    
    mSuits = mBafuda.mSuits;//�X�[�c
}
*/
    
/*
void Table::setBaInfo(int cards[8][15]){//���ʎ󂯎��͎̂�D�ł���A5�s�ڈȉ��̏�̏����X�V����
    if(cards[5][4]>0)   baBit |=  IS_BA_ONSET;  //���
    else                baBit &= ~IS_BA_ONSET;
    if(cards[5][6]>0)   baBit |=  IS_BA_KAKUMEI;//�v��
    else                baBit &= ~IS_BA_KAKUMEI;
    if(cards[5][7]>0)   baBit |=  IS_BA_SHIBARI;//����
    else                baBit &= ~IS_BA_SHIBARI;
	
    if( isOnset() ){//��𗬂�
        purge();
    }
	//mPlayers.update(cards);
}
*/

void Table::print() const{
    
    cout << "ON" << isOnset() 
     <<", KA" << isKaidan()
     <<", PA" << isPair()
     << ", TN" << isTanki() 
     << ", NU" << mBafuda.mNum << ", RNL" << mBafuda.mRankL<< ", RNR" << mBafuda.mRankR << endl;
    cout << "KK" << isKakumei() 
    << ", SH"<< isShibari() 
    << ", SS " << mBafuda.mSuits << endl;
    //cout << "bafuda" << endl;
    //int temp[8][15]={{0}};
    //mCard.printBit2();
    //mPlayers.print();
}

void Table::print2(){
    cout << "OKPTSJ|";
    cout << isOnset() << isKaidan() << isPair() << isTanki() << isShibari() << isJTanki();
    
}

