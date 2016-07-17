#include <iostream>
#include <algorithm>
#include "Yaku.h"
#include "makeYaku.h"

using namespace std;

//�������悤�̊֐��Q
//������joker1�����[���݂̂ɑΉ����Ă��邽�߁A�̂��̂��g�[����

void makeAllYaku(vector<Yaku> *allYaku, const int hands[8][15]){
    //hands�����邱�Ƃ��\�Ȃ��ׂĂ̖������
    //cout<<"a "<<allYaku->size()<<endl;
	makeKaidanFrom815(allYaku, hands);//�K�i
/*
int pa=0;
    for(int i=0;i<allYaku->size();i++){
    if((*allYaku)[i].isPass())pa++;
}    
cout<<"b "<<allYaku->size()<<" " <<pa<<endl;
*/
    makePairFrom815(allYaku, hands);//�y�A
/*
pa=0;
for(int i=0;i<allYaku->size();i++){
    if((*allYaku)[i].isPass())pa++;
}    
cout<<"c "<<allYaku->size()<<" " <<pa<<endl;
*/
    makeTankiFrom815(allYaku, hands);//�P�R
/*
pa=0;
for(int i=0;i<allYaku->size();i++){
    if((*allYaku)[i].isPass())pa++;
}    
cout<<"d "<<allYaku->size()<<" " <<pa<<endl;
*/
    makePass(allYaku);//�p�X
/*    
pa=0;
for(int i=0;i<allYaku->size();i++){
    if((*allYaku)[i].isPass())pa++;
}    
cout<<"d "<<allYaku->size()<<" " <<pa<<endl;
*/
}

void pickKaidan(vector<Yaku> *kaidan, const vector<Yaku> &allYaku){
    for(int i=0;i<allYaku.size();i++){
        if( allYaku[i].isKaidan() ){
            kaidan->push_back( allYaku[i] );
        }
    }
}

void pickPair(vector<Yaku> *pair, const vector<Yaku> &allYaku){
    for(int i=0;i<allYaku.size();i++){
        if( allYaku[i].isPair() ){
            pair->push_back( allYaku[i] );
        }
    }
}

void pickTanki(vector<Yaku> *tanki, const vector<Yaku> &allYaku){
    for(int i=0;i<allYaku.size();i++){
        if( allYaku[i].isTanki() ){
            tanki->push_back( allYaku[i] );
        }
    }
}

void pickLegalKaidan(vector<Yaku> *legalYaku, const vector<Yaku> &allYaku, const Table &table){
    int i;
    
    if(table.isKakumei()){//�v���̂Ƃ�
        for(i=0;i<allYaku.size();i++){
            //�K�i�Ŗ����������ł������̋�������D�̋������傫��
            if( allYaku[i].isKaidan() && 
                (allYaku[i].mNum==table.mBafuda.mNum && allYaku[i].mRankR<table.mBafuda.mRankL)
                ){
                if( !table.isShibari() || (table.isShibari() && table.mBafuda.mSuits==allYaku[i].mSuits)){
                    legalYaku->push_back( allYaku[i] );
                }
            }
        }
    }else{//�v���ł͂Ȃ��Ƃ�
        for(i=0;i<allYaku.size();i++){
            //�K�i�Ŗ����������ł������̋�������D�̋������傫��
            if( (allYaku[i].isKaidan()) &&
                 allYaku[i].mNum==table.mBafuda.mNum && allYaku[i].mRankL>table.mBafuda.mRankR)
                {
                if( !table.isShibari() || (table.isShibari() && table.mBafuda.mSuits==allYaku[i].mSuits)){
                    legalYaku->push_back( allYaku[i] );
                }
            }
        }
    }
}
	    
void pickLegalPair(vector<Yaku> *legalYaku, const vector<Yaku> &allYaku, const Table &table){
    int i;
    
    if(table.isKakumei()){//�v���̂Ƃ�
        for(i=0;i<allYaku.size();i++){
            //�K�i�Ŗ����������ł������̋�������D�̋������傫��
            if( allYaku[i].isPair() && 
               allYaku[i].mNum==table.mBafuda.mNum && allYaku[i].mRankL<table.mBafuda.mRankL
               ){
                if( !table.isShibari() || (table.isShibari() && table.mBafuda.mSuits==allYaku[i].mSuits)){
                    legalYaku->push_back( allYaku[i] );
                }
            }
        }
    }else{//�v���ł͂Ȃ��Ƃ�
        for(i=0;i<allYaku.size();i++){
            //�K�i�Ŗ����������ł������̋�������D�̋������傫��
            if( (allYaku[i].isPair()) && 
                allYaku[i].mNum==table.mBafuda.mNum && allYaku[i].mRankR>table.mBafuda.mRankL
                ){
                if( !table.isShibari() || (table.isShibari() && table.mBafuda.mSuits==allYaku[i].mSuits)){
                    legalYaku->push_back( allYaku[i] );
                }
            }
        }
    }
}
	   
void pickLegalTanki(vector<Yaku> *legalYaku, const vector<Yaku> &allYaku, const Table &table){
    int i;
    
    if(table.isJTanki()){//�W���[�J�[�P�R�̓X�y�R�����l�����Ȃ�
        for(i=0; i < allYaku.size(); i++){
            if(allYaku[i].isSpade3()==1){
                legalYaku->push_back( allYaku[i] );
                break;
            }
        }
    }
    else if(table.isKakumei()){//�v���̂Ƃ�
        for(i=0; i < allYaku.size(); i++){
            //�K�i�Ŗ����������ł������̋�������D�̋������傫��
            if(allYaku[i].mNum==1 && allYaku[i].mRankL<table.mBafuda.mRankL){
                if( !table.isShibari() || (table.isShibari() && table.mBafuda.mSuits==allYaku[i].mSuits || allYaku[i].isJTanki() )){
                    legalYaku->push_back( allYaku[i] );
                }
            }
        }
    }
    else{//�v���ł͂Ȃ��Ƃ�
        for(i=0; i < allYaku.size(); i++){
            //�K�i�Ŗ����������ł��E���̋�������D�̋������傫��
            if(allYaku[i].mNum==1 && allYaku[i].mRankR>table.mBafuda.mRankL){
                if( !table.isShibari() || (table.isShibari() && table.mBafuda.mSuits==allYaku[i].mSuits || allYaku[i].isJTanki() )){
                   legalYaku->push_back( allYaku[i] );
                }
            }
        }
    }
}
	    
void pickAllLegalYaku(vector<Yaku> *legalYaku, const vector<Yaku> &allYaku, const Table &table){
    //�����̎�D����\���ł���S���@��̂���
    //���݂̏�̏󋵂ɂ����āA�itable�j
    //�o�������݂̂��s�b�N�A�b�v����
    if(table.isOnset()){
    	//�p�X�������O����i���ł̓p�X�͂��ׂ��łȂ��ƐM���Ă���j
    	for(int i=0;i<allYaku.size()-1;i++){
    		legalYaku->push_back( allYaku[i] );
    	}
    }else{//�e�X�̏�ɑ΂���
    	if(table.isTanki()){
    		pickLegalTanki(legalYaku, allYaku, table);
    	}
    	else if(table.isPair()){
    		pickLegalPair(legalYaku, allYaku, table);
    	}
    	else if(table.isKaidan()){
    		pickLegalKaidan(legalYaku, allYaku, table);
    	}
    }
}

void makeKaidanFrom815(vector<Yaku> *yaku, const int hands[8][15]){
    //hands�����邱�Ƃ̉\�ȊK�i����yaku�ɓ����
	//�L�q���i�G�ɂȂ�̂ŏ�̏󋵂͈�؍l���Ȃ�
	//���Ƃō��v������̂������o��
	int joker = (hands[4][1]!=0);
	
	int suit, rank;
	for(suit=0; suit<4; suit++){//�X�[�g���ƈ�񂸂���
		for(rank=0; rank<13;rank++){//�n�_�����肷��i���蓾��̂�[12-13-14]�}�f�j
			int length = 0;         //�L�т钷���i���̖����j
			int joker_flag = joker; //joker�͎g���邩�B
			int searching = 1;      //�T����
			while(searching==1){
				if(rank+length<15 && hands[suit][rank+length]==1){//�J�[�h������Ȃ�
					length++;
				}else if(joker_flag==1){//�Ȃ��Ă��W���[�J�[���g����Ȃ�
					length++;
					joker_flag = 0;
				}else{//���Ȃ��ꍇ�͂����܂�
					searching = 0;
				}
				if(searching==1 && length>=3){//�K�i������
					int64 hd = IS_KAIDAN;
				    //hd |= (length<6) ? IS_NUM(length) : IS_NUM(6);//6?���܂ł̓r�b�g���g����
					int i, JposSuit=-1, JposRank=-1;
					int bitSuit = 0;  bitSuit|=(1<<suit);
					for(i=rank;i<rank+length;i++){
						if(hands[suit][i]==1){//1�Ȃ��1~13�Ȃ�
							hd |= CARDBIT(suit, i);
						}else{//�Ȃ����joker���g��
			                hd |= IS_JUSED;
							JposSuit = suit;JposRank = i;
						}
					}
				    
				    //if( rank<=6 && 6<=rank+length-1 )hd|=IS_8GIRI;//8�؂舵���ɂȂ�
					Yaku cd(hd, bitSuit, length, rank, rank+length-1, JposSuit, JposRank);
					yaku->push_back(cd);

					if(joker_flag==1){//joker���g�킸�ɗ��Ă���΁Ajoker�ɒu���������K�i����邱�Ƃ��ł���
						for(i=rank;i<rank+length;i++){
							int64 mask = ~CARDBIT(suit, i);//joker�g���Ƃ�����0
						    int64 hdj = (mask&hd)|IS_JUSED;//�c�����͎̂c��
							JposSuit=suit, JposRank=i;//joker�|�W�V�����͕ʕϐ��̂��ߑS�̂�ς���K�v�͂Ȃ��i�̂��H�j
							Yaku cd(hdj, bitSuit, length, rank, rank+length-1, JposSuit, JposRank);
							yaku->push_back(cd);
					    }
					}

				}
			}//while(searching)
		}//for(order
	}//for(suit
}

void makePairFrom815(vector<Yaku> *yaku, const int hands[8][15]){
	//�L�q���i�G�ɂȂ�̂ŏ�̏󋵂͈�؍l���Ȃ�
	//���Ƃō��v������̂������o��
	
	#define PATTERN_NUM 11
	int PATTERN[PATTERN_NUM][4] = {
		{1,1,0,0},{1,0,1,0},{1,0,0,1},{0,1,1,0},{0,1,0,1},{0,0,1,1},//6��2��
		{1,1,1,0},{1,1,0,1},{1,0,1,1},{0,1,1,1},                    //4��3��
		{1,1,1,1}                                                   //1��4��
	};//�y�A�̊e�X�[�g�̉\���p�^�[��
	//����ɍ��v����悤�ɒT���Ă���
	
	int suit, rank, pat;
    int joker = (hands[4][1]!=0);
	for(rank=1;rank<=13;rank++){//�e�����N�ɂ��Ă݂Ă���
		for(pat=0;pat<PATTERN_NUM; pat++){//���ꂼ��̃p�^�[���ɂ���
			int num = 0, match = 0, unmatch = 0;
			for(suit=0;suit<4;suit++){//�X�[�g�𒲂ׂĂ���
				if(PATTERN[pat][suit]==1){//���Y�̃p�^�[���ɂ���
					num++;//����
					if(hands[suit][rank]==1){//��D����o����
						match++;
					}else{//�o���Ȃ�
						unmatch++;
					}
				}
			}//for(suit
			if(num == match){//joker���g�킸�ɍ�邱�Ƃ��ł���
				int64 hd = IS_PAIR;
				int bitSuit = 0;
				for(suit=0;suit<4;suit++){
					if(PATTERN[pat][suit]==1){
						//hd |= CARDBIT(suit, rank)|IS_NUM(num);
					    hd |= CARDBIT(suit, rank);
						bitSuit |= (1<<suit);
					}
				}
			    //if(rank==6)hd|=IS_8GIRI;//8�؂�
				Yaku cd(hd, bitSuit, num, rank, rank, -1, -1);
				yaku->push_back(cd);
				if(joker==1){//joker�ɒu���������o�[�W�������i8�Ŏg�����炢�������_���킩��Ȃ��j
				    //hd |= 0ull;
					for(suit=0;suit<4;suit++){//joker�ɒu��������X�[�g
						if(PATTERN[pat][suit]==1){
						    int64 mask = ~CARDBIT(suit, rank);//�W���[�J�[���g���Ƃ��낾��0
						    //int64 hdj = (mask&hd)|IS_JUSED|IS_NUM(num);
						    int64 hdj = (mask&hd)|IS_JUSED;
							int JposSuit=suit, JposRank=rank;//joker�̈ʒu
							Yaku cdj(hdj, bitSuit, num, rank, rank, JposSuit, JposRank);
				            yaku->push_back(cdj);
						}
					}
				}
			}else if(joker==1 && unmatch==1){//joker���g���āA����Ȃ��̂�1�������Ȃ�
				//int64 hd = IS_PAIR|IS_JUSED|IS_NUM(num);
				int64 hd = IS_PAIR|IS_JUSED;
			    int bitSuit = 0;
				int JposSuit=-1, JposRank=-1;
				for(suit=0;suit<4;suit++){
					if(PATTERN[pat][suit]==1){
					    bitSuit |= (1<<suit);
						if(hands[suit][rank] == 1){
							hd |= CARDBIT(suit, rank);
						}else{
							JposSuit=suit, JposRank=rank;
						}
					}
				}
				Yaku cd(hd, bitSuit, num, rank, rank, JposSuit, JposRank);
				yaku->push_back(cd);
			}
		}
		
		//�Ō��5�J�[�h
		if(hands[0][rank] && hands[1][rank] && hands[2][rank] && hands[3][rank] && joker){//�S���o����
		    //int64 hd = IS_PAIR|IS_JUSED|IS_NUM(5);
		    int64 hd = IS_PAIR|IS_JUSED;
		    int JposSuit=4, JposRank=rank;
			hd|=CARDBIT(0, rank);hd|=CARDBIT(1, rank);hd|=CARDBIT(2, rank);hd|=CARDBIT(3, rank);
			Yaku cd(hd, 15, 5, rank, rank, JposSuit, JposRank);
			yaku->push_back(cd);
		}
        
	}//for(rank
}

void makeTankiFrom815(vector<Yaku> *yaku, const int hands[8][15]){
    //815�z�񂩂�P�R�������
    int suit, rank;
    int joker = (hands[4][1]!=0);//joker�������Ă���
	
	for(suit=0;suit<4;suit++){
		for(rank=1;rank<=13;rank++){
			if(hands[suit][rank]==1){
			    int bitSuit = (1<<suit);
			    //int64 hd = CARDBIT(suit, rank)|IS_NUM(1);
			    int64 hd = CARDBIT(suit, rank)|IS_TANKI;
				Yaku cd(hd, bitSuit, 1, rank, rank, -1, -1);
			    yaku->push_back(cd);
			}
		}
	}
	
	//joker�P�R�����̌���
	if(joker){
	    //int64 hd = IS_JUSED|IS_NUM(1);
	    int64 hd = IS_JUSED|IS_TANKI;
		Yaku cd(hd, 0, 1, 0, 14, 0, 14);
	    yaku->push_back(cd);
	}
}

void printYakuVec(const vector<Yaku> &vecCards){
    for(int i=0;i<vecCards.size();i++){
        cout << i << endl;
        vecCards[i].printBit();
        cout << endl;
    }
}
void makePass(vector<Yaku> *yaku){
    //���W���Ƀp�X������
	int64 hd = IS_PASS;
	Yaku cd(hd, 0, 0, 0, 0, -1, -1);
	yaku->push_back(cd);
}

/*
void sortYakuByRank( vector<Yaku> *vecCards, bool isKakumei ){
    //�ア���ɕ��ёւ���
    if( isKakumei ){//�v���̂Ƃ�
        sort( vecCards->begin(), vecCards->end(), isCard1WeakerThanCard2WhenKakumei );
    }else{
        sort( vecCards->begin(), vecCards->end(), isCard1WeakerThanCard2WhenNotKakumei );
    }
}
*/	    

void makeYakuBFrom815(vector<Yaku> *tky, int hands[8][15], const Table &table){
    
    if(table.isOnset()){//���
        makeKaidanFrom815(tky, hands);
        //std::cout << "kaidan" << tky->size() << std::endl;
        makePairFrom815(tky, hands);
        //std::cout << "pair" << tky->size() << std::endl;
        makeTankiFrom815(tky, hands);
        //std::cout << "tanki" << tky->size() << std::endl;
        //printVCard(tky);
    }else{
        if(table.isKaidan()){
            vector<Yaku> atky;
            makeKaidanFrom815(&atky, hands);
            //std::cout << "ak" << atky.size() << std::endl;
            //printVCard(&atky);
            sortKaidan(tky, &atky, table);
            //std::cout << "bk" << tky->size() << std::endl;
            //printVCard(tky);
        }else if(table.isPair()){
            vector<Yaku> atky;
            makePairFrom815(&atky, hands);
            //std::cout << "ap" << atky.size() << std::endl;
            //printVCard(&atky);
            sortPair(tky, &atky, table);
            //std::cout << "bp" << tky->size() << std::endl;
            //printVCard(tky);
        }else if(table.isTanki()){
            vector<Yaku> atky;
            makeTankiFrom815(&atky, hands);
            //printVCard(&atky);
            //std::cout << "at" << atky.size() << std::endl;
            sortTanki(tky, &atky, table);
            //std::cout << "bt" << tky->size() << std::endl;
            //printVCard(tky);
            
        }
        //printVCard(tky);
        makePass(tky);
    }
}	    
void sortAllYaku(vector<Yaku> *tky, const vector<Yaku> *atky, const Table &table){
    
    if(table.isOnset()){
    	//�p�X�������O����
    	for(int i=0;i<atky->size()-1;i++){
    		tky->push_back( (*atky)[i] );
    	}
    }else{
    	if(table.isTanki()){
    		sortTanki(tky, atky, table);
    	}
    	else if(table.isPair()){
    		sortPair(tky, atky, table);
    	}
    	else if(table.isKaidan()){
    		sortKaidan(tky, atky, table);
    	}
    }
	
}

void sortKaidan(vector<Yaku> *tky, const vector<Yaku> *atky, const Table &table){
    if(table.isKakumei()){//�v���̂Ƃ�
        for(int i=0;i<atky->size();i++){
            //�K�i�Ŗ����������ł������̋�������D�̋������傫��
            if( (*atky)[i].isKaidan() && 
                ((*atky)[i].mNum==table.mBafuda.mNum && (*atky)[i].mRankR<table.mBafuda.mRankL)
                ){
                if( !table.isShibari() || (table.isShibari() && table.mBafuda.mSuits==(*atky)[i].mSuits)){
                    tky->push_back((*atky)[i]);
                }
            }else if((*atky)[i].mNum==0){//�p�X�͎c��
                tky->push_back((*atky)[i]);
            }
        }
    }else{//�v���ł͂Ȃ��Ƃ�
        for(int i=0;i<atky->size();i++){
            //�K�i�Ŗ����������ł������̋�������D�̋������傫��
            if( ((*atky)[i].isKaidan()) &&
                 (*atky)[i].mNum==table.mBafuda.mNum && (*atky)[i].mRankL>table.mBafuda.mRankR)
                {
                if( !table.isShibari() || (table.isShibari() && table.mBafuda.mSuits==(*atky)[i].mSuits)){
                    tky->push_back((*atky)[i]);
                }
            }else if((*atky)[i].mNum==0){//�p�X�͎c��
                tky->push_back((*atky)[i]);
            }
        }
    }
}

void sortPair(vector<Yaku> *tky, const vector<Yaku> *atky, const Table &table){
    if(table.isKakumei()){//�v���̂Ƃ�
        for(int i=0;i<atky->size();i++){
            //�K�i�Ŗ����������ł������̋�������D�̋������傫��
            if( ((*atky)[i].isPair()) && 
               (*atky)[i].mNum==table.mBafuda.mNum && (*atky)[i].mRankL<table.mBafuda.mRankL
               ){
                if( !table.isShibari() || (table.isShibari() && table.mBafuda.mSuits==(*atky)[i].mSuits)){
                    tky->push_back((*atky)[i]);
                }
            }else if((*atky)[i].mNum==0){//�p�X�͎c��
                tky->push_back((*atky)[i]);
            }
        }
    }else{//�v���ł͂Ȃ��Ƃ�
        for(int i=0;i<atky->size();i++){
            //�K�i�Ŗ����������ł������̋�������D�̋������傫��
            if( ((*atky)[i].isPair()) && 
                (*atky)[i].mNum==table.mBafuda.mNum && (*atky)[i].mRankR>table.mBafuda.mRankR
                ){
                if( !table.isShibari() || (table.isShibari() && table.mBafuda.mSuits==(*atky)[i].mSuits)){
                    tky->push_back((*atky)[i]);
                }
            }else if((*atky)[i].mNum==0){//�p�X�͎c��
                tky->push_back((*atky)[i]);
            }
        }
    }
}

void sortTanki(vector<Yaku> *tky, const vector<Yaku> *atky, const Table &table){
    
    if(table.isJTanki()){//�W���[�J�[�P�R�̓X�y�R�����l�����Ȃ�
        for(int i=0;i<atky->size();i++){
            if((*atky)[i].isSpade3()==1){//�X��3�������Ă���
                tky->push_back((*atky)[i]);
                break;
            }else if((*atky)[i].mNum==0){//�p�X�͎c��
                tky->push_back((*atky)[i]);
            }
        }
    }
    else if(table.isKakumei()){//�v���̂Ƃ�
        for(int i=0;i<atky->size();i++){
            //�K�i�Ŗ����������ł������̋�������D�̋������傫��
            if((*atky)[i].mNum==1 && (*atky)[i].mRankL<table.mBafuda.mRankL){
                if( !table.isShibari() || (table.isShibari() && table.mBafuda.mSuits==(*atky)[i].mSuits || (*atky)[i].isJTanki() )){
                    tky->push_back((*atky)[i]);
                }
            }else if((*atky)[i].mNum==0){//�p�X�͎c��
                tky->push_back((*atky)[i]);
            }
        }
    }
    else{//�v���ł͂Ȃ��Ƃ�
        for(int i=0;i<atky->size();i++){
            //�K�i�Ŗ����������ł������̋�������D�̋������傫��
            if((*atky)[i].mNum==1 && (*atky)[i].mRankR>table.mBafuda.mRankR){
                if( !table.isShibari() || (table.isShibari() && table.mBafuda.mSuits==(*atky)[i].mSuits || (*atky)[i].isJTanki() )){
                    tky->push_back((*atky)[i]);
                }
            }else if((*atky)[i].mNum==0){//�p�X�͎c��
                tky->push_back((*atky)[i]);
            }
        }
    }
}
	    
	    /*
void removeLap(vector<Yaku> *vecCard, int64 cdBit){
    //���W������Card���Ƃ菜��
    int i = 0;
    int64 mask = BITull(53)-1;//�J�[�h�̂Ԃ񂾂��i�J�[�h�̍����܂ł͌��Ȃ��j
    while( i < (*vecCard).size() ){//�J�[�h�W���̒���T�����Ă���
    	if( ( mask & (*vecCard)[i].getCardBit() & cdBit ) == 0 ){//bit�����Ԃ�Ȃ�
            i++;//����_��i�߂�
        }else{//�����
            (*vecCard).erase( (*vecCard).begin() + i);//�W�������菜��
        }
    }
}
*/
	    