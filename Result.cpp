#include "Result.h"
#include "common.h"
#include "debug.h"
#include<cstdio>
#include <iomanip>
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

void Results::push_back(const Result &result){
    results.push_back(result);
}

/*
void Results::calcScore( int num ){
    //何番が
    int scores[5][5]={{0}};
    for(int i=0; i<results.size(); i++){
        for(int j=0; j<5; j++){
            scores[results[i].mAgari[j]][j]++;
        }
    }
    if(num == 1){
        for(int i=0; i<5; i++){
            int temp = 0;
            for(int j=0; j<5; j++){
                temp += scores[i][j] * (5-j);
            }
            score.push_back( temp );
        }
    }else if(num == 2){
        for(int i=0; i<5; i++){
            int temp = 0;
            for(int j=0; j<5; j++){
                temp += scores[i][j] * (2-j);
            }
            score.push_back( temp );
        }
    }
}
*/

void Results::calcScore( int num ){
    if( num == 1){
        /*
        for(int i=0; i<1; i++){
            vector<int> temp;
            for(int j=0; j<results[i].mAMibun.size(); j++){
                temp.push_back( 5 - results[i].mAMibun[j]);
            }
            score.push_back( temp );
        }
        for(int i=1; i<results.size(); i++){
            vector<int> temp;
            for(int j=0; j<results[i].mAMibun.size(); j++){
                temp.push_back( score[score.size()-1][j] + 5 - results[i].mAMibun[j]);
            }
            score.push_back( temp );
        }*/
        for(int i=0; i<results.size(); i++){
            vector<int> temp;
            for(int j=0; j<results[i].mAMibun.size(); j++){
                temp.push_back( 5 - results[i].mAMibun[j]);
            }
            score.push_back( temp );
        }
    }
    else if(num == 2){
        for(int i=0; i<1; i++){
            vector<int> temp;
            for(int j=0; j<results[i].mAMibun.size(); j++){
                temp.push_back( 2 - results[i].mAMibun[j]);
            }
            score.push_back( temp );
        }
        for(int i=1; i<results.size(); i++){
            vector<int> temp;
            for(int j=0; j<results[i].mAMibun.size(); j++){
                temp.push_back(  score[score.size()-1][j] + 2 - results[i].mAMibun[j]);
            }
            score.push_back( temp );
        }
    }
}


void Result::setup(vector<Player> *player){
    //playersで保存するものは保存する
    vector<int> mibun;
    for(int i=0; i<player->size(); i++){
        mibun.push_back( (*player)[i].mibun );
    }
    mMibun = mibun;
}

void Result::finish(Players &players){
    //playersで保存するものは保存する
    vector<int> amibun;
    for(int i=0; i<players.agari.size(); i++){
        amibun.push_back( players.id[i].mibun );
    }
    mAMibun = amibun;
}

void Result::setup(Players *players){
    //playersで保存するものは保存する
    vector<int> mibun;
    for(int i=0; i<players->size(); i++){
        mibun.push_back( (*players).id[i].mibun );
    }
    mMibun = mibun;
    /*
    if(mMibun[0]==mMibun[1]){
        cout << " equal " << endl;
    }
    */
}

void Results::calcTransition(){
    
    for(int j=0; j<results[0].mMibun.size(); j++){
        vector< vector<int> > temp1;
        for(int i=0; i<6; i++){
            vector<int> temp( 5, 0 );
            temp1.push_back(temp);
        }
        transition.push_back(temp1);
    }
    
    for(int i=0; i<results.size(); i++){
        //cout << results[i].mMibun[0] << " " << results[i].mMibun[1] << endl;
        if(results[i].mMibun[0]==HEIMIN && results[i].mMibun[1]==HEIMIN){
            //平等ターンより始まった
            for(int k=0; k<results[i].mAgari.size(); k++){
                transition[results[i].mAgari[k]][5][k]++;
            }
        }else{
            //すでに身分がついていて、新しい身分に変わった
            for(int k=0; k<results[i].mAgari.size(); k++){
                transition[results[i].mAgari[k]][results[i].mMibun[k]][k]++;
            }
        }
    }
    
}

void Results::print(){
    /*
    vector<int> finscore;
    for(int i=0; i<5; i++){
        int temp = 0;
        for(int j=0; j<score[i].size(); j++){
            temp+=score[i][j];
        }
        finscore.push_back( temp );
    }
    */
    for(int i=0; i<5; i++){
        //cout << "Player " << i << " : " << score[i] << endl;
        int temp = 0;
        for(int j=0; j<score.size(); j++){
            temp += score[j][i];
        }
        //cout << i << ":" << name[i] << " = " << score[score.size()-1][i] << endl;
        cout << i << ":" << name[i] << " = " << temp << endl;
        
        for(int j=0; j<6; j++){
            int temp = 0;
            for(int k=0; k<5; k++){
                temp += transition[i][j][k];
                cout << transition[i][j][k] << " ";
            }
            cout << " (";
            for(int k=0; k<5; k++){
                cout << fixed << setprecision(1) << 100*(double)transition[i][j][k]/temp << " ";
            }
            cout << ") / " << temp << endl;
        }
    }
    cout << "all time = " << (double)mTime << "s" << endl;
    cout << "time ave. = " << (double)mTime/results.size() << "s" << endl;
}

Results::Results( Players &players ){
    for(int i=0; i<5; i++){
        string pn = players.id[i].name;
        name.push_back( pn );
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
