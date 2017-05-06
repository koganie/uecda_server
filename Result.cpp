#include<cstdlib>
#include<cstdio>
#include <iomanip>
#include<cmath>
#include "Result.h"
#include "common.h"
#include "debug.h"
#include "makeYaku.h"
using namespace std;

Action::Action( int id, const Yaku &yaku ){
    mId = id;
    mYaku = yaku;
}

Action::Action( int id, Table &bTable, vector<int> &passArray, vector<int> &numArray, const Yaku &yaku ){
    mId = id;
    mTable = bTable;
    mPA = passArray;
    mNA = numArray;
    mYaku = yaku;
}

Change::Change( int from, int to, const int cards[8][15] ){
    mFrom = from;
    mTo = to;
    for(int i=0; i<8; i++){
        for(int j=0; j<15; j++){
            mCards[i][j] = cards[i][j];
        }
    }
}

void Change::print(){
    cout << " change : " << mFrom << " " << mTo << endl;
    print815( mCards );
}

Result::Result(){
    sekigae = false;
    reset = false;
}

void Result::addAct( int id, const Yaku &yaku ){
    Action act( id, yaku );
    mHistory.push_back( act );
}

void Result::addAct( int id, Table &bTable, vector<int> &passArray, vector<int> &numArray, const Yaku &yaku ){
    Action act( id, bTable, passArray, numArray, yaku );
    mHistory.push_back( act );
}

void Result::addChange( int from, int to, const int cards[8][15] ){
    Change change( from, to, cards );
    mChange.push_back( change );
}

void Result::addAgari( int id ){
    mAgari.push_back( id );
}
void Result::addHands( const int cards[8][15]){
    vector< vector< int > > temp815;
    for(int i=0; i<8; i++){
        vector<int> temp1;
        for(int j=0; j<15; j++){
            temp1.push_back( cards[i][j] );
        }
        temp815.push_back( temp1 );
    }
    mHands.push_back( temp815 );
}

void Result::setMibun( vector<Player> *players ){
    for(int i=0; i<mAgari.size(); i++){
        (*players)[i].mibun = i;
    }
}

void Result::update( Players *players ){
    //cout<<"update"<<endl;
    for(int i=0; i<mAMibun.size(); i++){
        //cout << (*players).id[i].mibun << " " << mAMibun[i] << endl;
        (*players).id[i].mibun = mAMibun[i];
    }
    
}

void Results::clockStart(){
    mStart = clock();
}

void Results::clockEnd(){
    mEnd = clock();
    mTime = (double)(mEnd - mStart) / CLOCKS_PER_SEC;
}

/*
void Results::printOneGameReport(){
    if(results[results.size()-1].reset){
        cout << "reset" << endl;
    }else if(results[results.size()-1].sekigae){
        cout << "sekigae" << endl;
    }
    cout << results.size() <<" gameEnd" << endl;
    for(int i=0; i<results[results.size()-1].mMibun.size(); i++){
        cout << results[results.size()-1].mMibun[i] << " => " << results[results.size()-1].mAMibun[i] << endl;
    }
}
*/

void printCardStr( int cards[8][15] ){
    string suit[5] = "DSCH";
    for(int j=0; j<=3; j++){
        for(int k=1; k<=13; k++){
            if(cards[j][k]!=0){
                cout << suit[j] << k;
            }
        }
    }
    for(int j=0; j<15; j++){
        if(cards[4][j]!=0){
            cout << "J";
        }
    }
}

void printCardStr( vector< vector<int> > cards ){
    string suit[5] = "DSCH";
    for(int j=0; j<=3; j++){
        for(int k=1; k<=13; k++){
            if(cards[j][k]!=0){
                cout << suit[j] << k;
            }
        }
    }
    for(int j=0; j<15; j++){
        if(cards[4][j]!=0){
            cout << "J";
        }
    }
}

void printArray( const vector<int> &array ){
    cout << "|";
    for(int i=0; i<array.size(); i++){
        cout << array[i];
    }
    cout << "|";
}

void printArray2( const vector<int> &array ){
    cout << "|";
    for(int i=0; i<array.size(); i++){
        printf("%2d", array[i]);
    }
    cout << "|";
}

void Result::print(){
    
    cout << "first Cards" << endl;
    for(int i=0; i<mHands.size(); i++){
        cout << "Player " << i << endl;
        printCardStr( mHands[i] );
        cout << endl;
    }
    if(mChange.size() > 0){
        cout << "change Cards" << endl;
        for(int i=0; i<mChange.size(); i++){
            cout << mChange[i].mFrom << " -> " << mChange[i].mTo << endl;
            printCardStr( mChange[i].mCards );
            cout << endl;
        }
    }else{
        cout << "no change" << endl;
    }
    cout << mHistory.size() << endl;
    for(int i=0; i<mHistory.size(); i++){
        printf("%00d:", i);
        cout << mHistory[i].mId << ":";
        mHistory[i].mTable.print2();//提出前の盤面情報
        printArray(mHistory[i].mPA);//パスアレイ
        printArray2(mHistory[i].mNA);//枚数アレイ
        mHistory[i].mYaku.print();//提出役
        cout << endl;//開業
    }
    for(int i=0; i<mMibun.size(); i++){
        cout << mMibun[i] << " ";
    }
    cout << endl;
    for(int i=0; i<mAMibun.size(); i++){
        cout << mAMibun[i] << " ";
    }
    cout << endl;
}

void Result::printOneGameReport( int num ){
    cout << num << " games end"<<endl;
    if(reset){
        cout << "reset" << endl;
    }else if(sekigae){
        cout << "sekigae" << endl;
    }
    for(int i=0; i<mMibun.size(); i++){
        cout << mMibun[i] << " => " << mAMibun[i] << endl;
    }
}

void Results::push_back(const Result &result){
    //試合結果の登録
    size += 1;
    //results.push_back(result);
    mMibun.push_back( result.mMibun );//試合前の身分を登録
    mAMibun.push_back( result.mAMibun );//試合後の身分を登録
    
}

void Results::calcScore( int num ){
    int point[2][5] = {
        {5,4,3,2,1},    //大貧民1点、・・・、大富豪5点
        {2,1,0,-1,-2}   //大貧民-2点、・・・、大富豪2点
    };
    
    for(int i=0; i<size; i++){
        vector<int> temp;
        for(int j=0; j<mAMibun[0].size(); j++){
            temp.push_back( point[num][mAMibun[i][j]] );
        }
        score.push_back( temp );
    }

}

void Result::finish(Players &players){
    //playersで保存するものは保存する
    vector<int> amibun;
    for(int i=0; i<players.agari.size(); i++){
        amibun.push_back( players.id[i].mibun );
    }
    mAMibun = amibun;
}

void Result::setup(Players *players, Configure &c){
    //playersで保存するものは保存する
    vector<int> mibun, sekijun;
    for(int i=0; i<players->size(); i++){
        mibun.push_back( (*players).id[i].mibun );
        sekijun.push_back( (*players).sekijun[i] );
    }
    mMibun = mibun;
    mSekijun = sekijun;
    mConfig = c;
    /*
    if(mMibun[0]==mMibun[1]){
        cout << " equal " << endl;
    }
    */
}


void Result::setFirstCards( int cards[5][8][15] ){
    for(int i=0; i<5; i++){
        vector< vector<int> > temp;
        for(int j=0; j<5; j++){
            vector<int> temp1;
            for(int k=0; k<15; k++){
                temp1.push_back( cards[i][j][k] );
            }
            temp.push_back( temp1 );
        }
        mHands.push_back( temp );
    }
}

void Results::calcTransition(){
    //身分の遷移を調べる
    
    //transition配列のセットアップ
    for(int j=0; j<mMibun[0].size(); j++){
        vector< vector<int> > temp1;
        for(int i=0; i<6; i++){
            vector<int> temp( 5, 0 );
            temp1.push_back(temp);
        }
        transition.push_back(temp1);
    }
    
    //遷移を調べる
    for(int i=0; i<mAMibun.size(); i++){
        if(mMibun[i][0]==HEIMIN && mMibun[i][1]==HEIMIN){
            //平等ターンより始まった
            for(int k=0; k<mAMibun[i].size(); k++){
                transition[k][5][mAMibun[i][k]]++;
            }
        }else{
            //すでに身分がついていて、新しい身分に変わった
            for(int k=0; k<mAMibun[i].size(); k++){
                transition[k][mMibun[i][k]][mAMibun[i][k]]++;
            }
        }
    }
    
}

void Results::print(){
    string mibun[6] = {"DFU","FU","HE","HI","DHI","BYD"};
    
    int maxKeta = 0, printKeta=0;
    for(int i=0; i<5; i++){
        for(int j=0; j<6; j++){
            for(int k=0; k<5; k++){
                if( log10(transition[i][j][k])+1 > maxKeta ){
                    maxKeta = (int)log10(transition[i][j][k])+1;
                }
            }
        }
    }
    
    for(int i=0; i<5; i++){
        //cout << "Player " << i << " : " << score[i] << endl;
        int temp = 0;
        for(int j=0; j<score.size(); j++){
            temp += score[j][i];
        }
        //cout << i << ":" << name[i] << " = " << score[score.size()-1][i] << endl;
        cout << i << ":" << name[i] << " = " << temp << endl;
        cout << "   ";
        if( maxKeta > 4 ){
            printKeta = maxKeta;
        }else{
            printKeta = 4;
        }
        for(int j=0; j<5; j++){
            cout << setw(printKeta) << mibun[j];
        }
        cout << endl;
        
        if( maxKeta > 3 ){
            printKeta = maxKeta;
        }else{
            printKeta = 3;
        }
        for(int j=0; j<6; j++){
            int temp = 0;
            cout << setw(printKeta) << mibun[j] << ":";
            for(int k=0; k<5; k++){
                temp += transition[i][j][k];
                cout << setw(printKeta) << transition[i][j][k] << " ";
            }
            cout << " (";
            for(int k=0; k<5; k++){
                if(temp != 0){
                    cout << setw(4) << fixed << setprecision(1) << 100*(double)transition[i][j][k]/temp << " ";
                }else{
                    cout << "--- ";
                }
            }
            cout << ") / " << temp << endl;
        }
    }
    cout << "all time = " << (double)mTime << "s" << endl;
    cout << "time ave. = " << (double)mTime/size << "s" << endl;
}

Results::Results( Players &players ){
    size = 0;
    for(int i=0; i<5; i++){
        string pn = players.id[i].name;
        name.push_back( pn );
        //tempScore.push_back( 0 );
    }
}

void Results::writeScore(){
    ofstream ofs("score.txt");
    for(int i=0; i<name.size(); i++){
        ofs << name[i] << " ";
    }
    ofs << endl;
    for(int i=0; i<score.size(); i++){
        for(int j=0; j<score[i].size(); j++){
            ofs << score[i][j] << " ";
        }
        ofs << endl;
    }
}

void Results::writeTransition(){
    ofstream ofs("transition.txt");
    for(int i=0; i<name.size(); i++){
        ofs << name[i] << endl;
        for(int j=0; j<transition[i].size(); j++){
            for(int k=0; k<transition[i][j].size(); k++){
                ofs << transition[i][j][k] << " ";
            }
            ofs << endl;
        }
    }
}

string Change::getStr(){
    string str;
    string suit = "SHDCJ";
    string rank = "3456789XJQKA2R";
    
    ostringstream stream1;
    stream1 << mFrom;
    ostringstream stream2;
    stream2 << mTo;
    str += stream1.str();
    str += " ";
    str += stream2.str();
    str += " ";
    for(int i=0; i<5; i++){
        for(int j=0; j<15; j++){
            ostringstream stream;
            if(mCards[i][j]==1){
                str += suit[i];
                //stream << j;
                //str += stream.str();
                str += rank[j-1];
                str += " ";
            }else if(mCards[i][j]==2){
                str += "JR ";
            }
            
        }
    }
    return str;
}

void Results::writeHistory(){
    //棋譜をファイル出力する
    
    ofstream ofs("history.txt", std::ios::out);
   
    //プレイヤーの名前
    for(int i=0; i<name.size(); i++){
        ofs << i << " " << name[i] << endl;
    }
    ofs << endl;
}
void Result::writeHistory(){
    //棋譜をファイル出力する
    //形式はとりあえず適当に書いておく
    
    ofstream ofs("history.txt", std::ios::app);
    string suit = "SHDCJ";
    string rank = "3456789XJQKA2R";
    
    //ofs << mConfig.getStr() << endl;//その試合のルールの出力
    
    //最初の手札の出力
    ofs << "TEFUDA" << endl;
    for(int j=0; j<mHands.size(); j++){
        ofs << j << " " << mMibun[j] << " " << mSekijun[j] << " ";//プレイヤー番号と身分と席順

        for(int k=0; k<4; k++){
            for(int l=0; l<15; l++){
                if(mHands[j][k][l]==1){
                    ofs << suit[k] << rank[l-1] << " ";
                }
            }
        }
        if(mHands[j][4][1]!=0){
            ofs << "JR";
        } 
        ofs << endl;
    }
    //ofs << endl;
    if( mChange.size() > 0 ){//交換が行われている
        ofs << "CHANGE" << endl;
        for(int j=0; j<mChange.size(); j++){
            //前後で盤面の出力をさせてもよい
            ofs << mChange[j].getStr() << endl;
        }
        //ofs << endl;
        /*
        ここで最終的な手札を出力させてもよい
        */
    }
    
    ofs << "HISTORY" << endl;
    //一試合を一行で書き連ねる
    for(int j=0; j<mHistory.size(); j++){
        //各手各手を出力させていく
        ofs << mHistory[j].mId;
        ofs << mHistory[j].mYaku.getStr() << " ";
    }
    ofs << endl;
    ofs << "MIBUN" << endl;//新しい身分（ゲームの結果）
    for(int j=0; j<mHands.size(); j++){
        ofs << j << " " << mAMibun[j] << endl;
    }
    ofs << endl;

}
