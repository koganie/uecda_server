#include<iostream>
#include<string.h>

#include<cstdlib>
#include <sstream>

#include "Yaku.h"
#include "bit.h"
#include "common.h"
#include "debug.h"

using namespace std;

Yaku::Yaku(int64 cd, int suits, int num, int rank, int rank2, int jps, int jpr){
    mCardBit = cd;//入力のcdの属性等はすでに出来上がったものを想定している
    
    mSuits = suits;
    mNum = num;
    mRankR = rank2;//右端
    mRankL = rank;  //左端
    mJposSuit = jps;
    mJposRank = jpr;
    
    //bzero(mCards, sizeof(mCards));
    memset(mCards, 0, sizeof(mCards));
    setBitTo815( mCards );//815配列にも格納する
    /*
    mKaidan=isKaidan();
    mPair=isPair();
    mTanki=isTanki();
    mPass=isPass();
    mJoker=isJTanki();
    
    //副作用
    mKakumei=isKakumei();
    m8giri=is8giri();
    */
}

bool Yaku::isKaidan() const{
    return (mCardBit&(IS_KAIDAN))!=0;
}
bool Yaku::isPair() const{
    return (mCardBit&(IS_PAIR))!=0;
}
bool Yaku::isTanki() const{
    return mNum==1;
}
bool Yaku::isJUsed() const{
    return ( (mCardBit&IS_JUSED)!=0 );//jokerが立っている
}
bool Yaku::isJTanki() const{
    //return ( (mCardBit&IS_JUSED)!=0 && (mCardBit&IS_NUM(1))!=0);//両方立っている
    return ( (mCardBit&IS_JUSED)!=0 && (mCardBit&IS_TANKI)!=0);//両方立っている
}
bool Yaku::isKakumei() const{
    if(isKaidan() && mNum>=5 || isPair() && mNum>=4)return 1;
    else return 0;
}

bool Yaku::is8giri() const{
    /*
    return (mCardBit&(IS_8GIRI))!=0;
    */
    if( mRankL <= 6 && 6 <= mRankR && !isJTanki() ){
        return true;
    }else{
        return false;
    }
}
bool Yaku::isSpade3() const{
    /*
    return (mCardBit&(IS_SPADE3))!=0;
    */
    if( mNum==1 && mRankR==1 && mRankL==1 && mSuits==1){
        return true;
    }else{
        return false;
    }
    
}
bool Yaku::isPass() const{
    return (mNum==0) ? 1 : 0;
}
int64 Yaku::getCardBit() const{
    return mCardBit;
}

void Yaku::setSuit(int s){
    mSuits |= BITull(s);
}


void Yaku::setTanki(){
    mCardBit |= IS_TANKI;
}

void Yaku::setPair(){
    mCardBit |= IS_PAIR;
}

void Yaku::setKaidan(){
    mCardBit |= IS_KAIDAN;
}
void Yaku::setJTanki(){
    mCardBit |= (IS_JUSED | IS_TANKI);
}

void Yaku::clear(){
    mCardBit = 0ull;
    mSuits = 0;
    mSpe3 = false;
    mNum = 0;
    mRankR = 0;
    mRankL = 0;
    mJposSuit = -1;
    mJposRank = -1;
}
    
void Yaku::init(){
    clear();
    mNum=0;mRankR=0;mRankL=0;
}

void Yaku::demoPass(){
    clear();
    mCardBit = IS_PASS;
    //bzero(mCards, sizeof(mCards));
    memset(mCards, 0, sizeof(mCards));
    setBitTo815( mCards );//815配列にも格納する
}
    
void Yaku::setBitTo815(int dest[8][15]) const{
    int i,j;
    for(i=0;i<4;i++){
        for(j=1;j<=13;j++){
            //if(mCardBit&BITull(i*13+(j-1))){
            if(mCardBit&CARDBIT(i, j)){
                dest[i][j]=1;
            }else{
                dest[i][j]=0;
            }
        }
    }
    if(mJposSuit>=0 && mJposRank>=0){
        dest[mJposSuit][mJposRank]=2;
    }
}

void Yaku::printBit() const{
    int temp[8][15]={{0}};
    std::cout << "kdn" << isKaidan() << ", pir" <<isPair() 
    << /*", jok" << isJoker() << */ ", kkm" << isKakumei()
    << /*", sbr" << isShibari() <<*/ ", 8g" << is8giri() 
    << ", num" << mNum << ", rktT" << mRankR << ", rkr" << mRankL 
    << ", suit" << mSuits << std::endl;
    cout << "pass" << isPass() << endl;
    cout << "isSpade3" << isSpade3() << endl;
    cout << "jr"<<mJposRank<<" " <<mJposSuit<<" "<<isJUsed()<<endl;
    cout << "a" << endl;
    setBitTo815(temp);
    cout << "b" << endl;
    //print515(temp);
    cout << "c" << endl;
}

void Yaku::printBit2() const{
    for(int i=0;i<4;i++){
        for(int j=1;j<=13;j++){
            if( mCardBit&BITull(i*13+(j-1)) ){
                std::cout << "1 ";
            }else{
                std::cout << "0 ";
            }
        }
        std::cout << std::endl;
    }
    /*
    std::cout << "S3" << isSpade3() << " JT" << isJTanki() << " KA" << isKaidan()
    << " PA" << isPair() << " PA" << isPass() << " 8G" << is8giri() << std::endl;
    for(int i=0;i<5;i++){
        std::cout << "N" << i << " " << ((mCardBit&IS_NUM(i))!=0) << ",";
    }
    std::cout << "N" << 5 << " " << ((mCardBit&IS_NUM(5))!=0) << std::endl;
    */
}

void Yaku::printBit3() const{
    std::string SUIT="SHDC";
    std::string RANK="-34567890JQKA2";
    for(int j=1;j<=13;j++){
        for(int i=0;i<4;i++){
            if( i==mJposSuit && j==mJposRank ){
                std::cout << SUIT[i] << RANK[j] << "*";
            }else if( (mCardBit&BITull(i*13+(j-1)))!=0 ){
                std::cout << SUIT[i] << RANK[j];
            }
        }
    }
}

void Yaku::set815ToBit(int src[8][15]){

    int i, j;
    mCardBit = 0ULL;
    for(i=0;i<4;i++){
        //jokerの位置
        if(src[i][0]==2){
            mJposSuit = i;
            mJposRank = 0;
        }
        if(src[i][14]==2){
            mJposSuit = i;
            mJposRank = 14;
        }
        //通常カードの位置
        for(j=1;j<=13;j++){
            if(src[i][j]==0){
            }else if(src[i][j]==1){
                //mCardBit |= BITull(i*13 + (j-1));
                mCardBit |= CARDBIT(i, j);
            }else if(src[i][j]==2){
                mJposSuit = i;
                mJposRank = j;
            }
        }
    }
    //jokerの位置
    for(j=0;j<=14;j++){
        if(src[4][j]==2){
            mJposSuit = 4;
            mJposRank = j;
        }
    }
    
}

void Yaku::setBit(int src[8][15]){
    //bitに埋め込む53項目
    mCardBit = 0ULL;
    mSuits = 0;
    set815ToBit(src);//cardをbitに入れ込む。
    
    int i=0,j=0,count=0;
    //カードのある位置を探す
    while(j<15&&src[i][j]==0){
	    i++;
		if(i==4){
    		j++;
	    	i=0;
	    }
    }
    
    int joker_flag=0;
    if(src[i][j] == 2) joker_flag = 1;
    //階段が否か
    if(j<14 && src[i][j+1]>0){
    	mCardBit |= IS_KAIDAN;
    }
    //枚数を数える また強さを調べる
   	if( isKaidan() ){
   		//階段
	    while(j+count<15 && src[i][j+count]>0){
		    if(j+count==6)  mCardBit |= IS_8GIRI;
		    count++;
	    }
   	    mRankR = j+count-1;
        mRankL = j;
	    mSuits = (1<<i);
    }else{
   		//枚数組
        for(;i<5;i++){
    		if(src[i][j]>0){
	    		count++;
		    	mSuits |= (1<<i);
		    }
	    }
   		if(count==1&&joker_flag==1){//joker単騎
    		mRankR = 14;
    		mRankL = 0;
   		}else{
    		mRankR = j;
    		mRankL = j;
	    }
	    if(count>1){//複数枚
	        mCardBit |= IS_PAIR;
	        //if(count>=4)mCardBit |= IS_KAKUMEI;
	    }else if(count==1){//単騎出しフラグはない
	        
	    }
	    if(j==6){//8切り
	        mCardBit |= IS_8GIRI;
	    }
   	}
    mNum = count;
    //mCardBit |= (count<7) ? IS_NUM(count) : IS_NUM(7);//6枚まではビットが使える
}
Yaku::Yaku(){
    clear();
}

void removeLap(vector<Yaku> *vecCard, int64 cdBit){
    //役集合からCardをとり除く
    int i = 0;
    int64 mask = BITull(53)-1;//カードのぶんだけ（カードの左側までは見ない）
    while( i < (*vecCard).size() ){//カード集合の中を探索していく
    	if( ( mask & (*vecCard)[i].getCardBit() & cdBit ) == 0 ){//bitがかぶらない
            i++;//操作点を進める
        }else{//被った
            (*vecCard).erase( (*vecCard).begin() + i);//集合から取り除く
        }
    }
}

void removeYaku(vector<Yaku> *vecCard, Yaku &yaku){
    //役集合からyakuをとり除く
    int i = 0;
    vector<Yaku> test;
    while( i < (*vecCard).size() ){//カード集合の中を探索していく
    	if( (*vecCard)[i].mNum==yaku.mNum && (*vecCard)[i].mRankL==yaku.mRankL &&
            (*vecCard)[i].mRankR==yaku.mRankR && (*vecCard)[i].mSuits==yaku.mSuits &&
            (*vecCard)[i].mJposSuit==yaku.mJposSuit && (*vecCard)[i].mJposRank==yaku.mJposRank 
        ){//同じ
            test.push_back((*vecCard)[i]);
            (*vecCard).erase( (*vecCard).begin() + i);//集合から取り除く
        }else{//被った
            i++;//操作点を進める
        }
    }
    if( test.size() > 1){
        cout<<"hen!"<< endl;
        yaku.print();
cout << yaku.mNum << " " << yaku.mRankL << " " << yaku.mRankR << " " << yaku.mSuits << endl;
cout << endl;
        for(int j=0; j<test.size(); j++){
            test[j].print();
    cout<<endl;        
}
    exit(1);
    }else if(test.size()==0){
cout<<"yapparihen"<<endl;
yaku.print();
cout << yaku.mNum << " " << yaku.mRankL << " " << yaku.mRankR << " " << yaku.mSuits << " " << yaku.mJposSuit << " " << yaku.mJposRank << endl;
cout<<"----"<<endl;
for(int j=0;j<(*vecCard).size();j++){
    (*vecCard)[j].print();
cout << (*vecCard)[j].mNum << " " << (*vecCard)[j].mRankL << " " << (*vecCard)[j].mRankR << " " << (*vecCard)[j].mSuits << " " << (*vecCard)[j].mJposSuit << " " << (*vecCard)[j].mJposRank << endl;cout<<endl;
}
exit(1);
    }
}

void removePass(vector<Yaku> *vecCard){
    //役集合からPassをとり除く
    int i = 0;
    while( i < (*vecCard).size() ){//カード集合の中を探索していく
        if( (*vecCard)[i].isPass() == true ){
            (*vecCard).erase( (*vecCard).begin() + i);//集合から取り除く
        }else{
            i++;
        }
    }
}

bool isCard1WeakerThanCard2WhenNotKakumei(const Yaku &c1, const Yaku &c2){
    //通常時（革命でない）とき、第1引数が第2引数よりも小さいとき1を返す
    return ( c1.mRankL < c2.mRankL);
}

bool isCard1WeakerThanCard2WhenKakumei(const Yaku &c1, const Yaku &c2){
    //革命のとき、第1引数が第2引数よりも小さいとき1を返す
    return ( c1.mRankR > c2.mRankR);
}

int selectBigCards(Yaku *card, const std::vector<Yaku> &vecCard){
    if(vecCard.size()==0){
        cout << "naka nai" << endl;
        exit(1);
    }else if(vecCard.size()==1){
        *card = vecCard[0];
        return 0;
    }
    int index = 0;
    int maxNum = vecCard[0].mNum;
    for(int i=1;i<vecCard.size();i++){
        if( vecCard[i].mNum > maxNum ){
            maxNum = vecCard[i].mNum;
            index = i;
        }
    }
    if(index >= 0){
        //cout<<"yyyy "<<index<<" "<<vecCard.size()<<endl;
        //vecCard[index].printBit();
        *card = vecCard[index];
        return index;
    }else{
        std::cout << "selectBigCards" << vecCard.size() << std::endl;
        exit(1);
    }
}

void Yaku::print() const{
    string suit = "SHDC ";
    int cards[8][15] = {{0}};
    //cout << " gg" << endl;
    setBitTo815( cards );
    //print815( cards );
    //cout << " bb " << endl;
    
    for(int j=0; j<=5; j++){
        for(int k=0; k<=14; k++){
            if(cards[j][k]==1){
                cout << suit[j] << k;
            }else if(cards[j][k]==2){
                cout << "J";
            }
        }
    }
    //cout<<endl;
    //cout << mNum << " " << mRankL << " " << mRankR << " " << mSuits << endl;
}

string Yaku::getStr(){
    
    string suit = "SHDCshdcJ";
    string rank = "B3456789XJQKA2U";
    
    int cards[8][15] = {{0}};
    setBitTo815( cards );
    string str="";
    
    if( isJTanki() ){
        str+="JR";
    }else if( isPass() ){
        str+="PS";
    }else{
        for(int j=0; j<=5; j++){
            for(int k=0; k<=14; k++){
                ostringstream stream;
                stream << k;
                if(cards[j][k]==1){
                    str += suit[j];
                    str += rank[k];
                    //str += stream.str();
                }else if(cards[j][k]==2){
                    str += suit[j+4];
                    str += rank[k];
                    //str += stream.str();
                }
            }
        }
    }
    
    return str;
    //cout<<endl;
    //cout << mNum << " " << mRankL << " " << mRankR << " " << mSuits << endl;
}
