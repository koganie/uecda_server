#ifndef SUBMIT_CARD_DEFINE
#define SUBMIT_CARD_DEFINE

#include <vector>
#include "Table.h"

using namespace std;

void makeAllYaku(vector<Yaku> *allYaku, const int hands[8][15]);//���ׂĂ̖������
void makeKaidanFrom815(vector<Yaku> *yaku, const int hands[8][15]);//�K�i�����
void makePairFrom815(vector<Yaku> *yaku, const int hands[8][15]);//�y�A�����
void makeTankiFrom815(vector<Yaku> *yaku, const int hands[8][15]);//�P�R�����
void makePass(vector<Yaku> *yaku);//�p�X�����

//�����͕s�v�ɂ���
void sortKaidan(vector<Yaku> *tky, const vector<Yaku> *atky, const Table &table);
void sortPair(vector<Yaku> *tky, const vector<Yaku> *atky, const Table &table);
void sortTanki(vector<Yaku> *tky, const vector<Yaku> *atky, const Table &table);
void sortAllYaku(vector<Yaku> *tky, const vector<Yaku> *atky, const Table &table);
void makeYakuBFrom815(vector<Yaku> *tky, int hands[8][15], const Table &table);
/*
//Yaku�z��̓Y���������������
void sortKaidan2(vector<int> *parallel, const vector<Yaku> *atky, const Table &table);
void sortPair2(vector<int> *parallel, const vector<Yaku> *atky, const Table &table);
void sortTanki2(vector<int> *parallel, const vector<Yaku> *atky, const Table &table);
void sortAllYaku2(vector<int> *parallel, const vector<Yaku> *atky, const Table &table);
*/

//allYaku���炷�ׂĂ̊K�i������Atable�͌��Ȃ�
void pickKaidan(vector<Yaku> *kaidan, const vector<Yaku> &allYaku);
void pickPair(vector<Yaku> *pair, const vector<Yaku> &allYaku);
void pickTanki(vector<Yaku> *tanki, const vector<Yaku> &allYaku);

//allYaku�̖��W������table�ɒ�o�\�Ȃ��̂�legalYaku�ɓ���Ă���
//makeAllYaku���g������ɂ���ɓn�����ƂŁA�S�Ă̍��@��W�������
void pickAllLegalYaku(vector<Yaku> *legalYaku, const vector<Yaku> &allYaku, const Table &table);
void pickLegalKaidan(vector<Yaku> *legalYaku, const vector<Yaku> &allYaku, const Table &table);
void pickLegalPair(vector<Yaku> *legalYaku, const vector<Yaku> &allYaku, const Table &table);
void pickLegalTanki(vector<Yaku> *legalYaku, const vector<Yaku> &allYaku, const Table &table);

void printYakuVec(const vector<Yaku> &vecYakus);
void makePass(vector<Yaku> *tky);

//���W�����ア���ɕ��ׂ�ivecCards[0]���Ŏ�ƂȂ�悤�ɂ���j
void sortYakuByRank( vector<Yaku> *vecYakus, bool isKakumei );

//void removeLap(vector<Yaku> *vecCard, int64 cdBit);
#endif