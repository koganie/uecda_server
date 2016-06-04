#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<iostream>
#include "Game.h"
#include "makeYaku.h"
#include "common.h"
#include "connection.h"

#include "debug.h"

Game::Game(const Configure &c, const Players &p, Result *result){
    //GAMEコンストラクタ
    //最初の手札を配分して、通知までする
    COUT<<"GAME"<<endl;
    
    config = c;//ゲームのルールを格納する
    players = p;//プレイヤーをコピーする
    
    for( int i=0; i<5; i++){
        //cout << players.id[i].mibun << endl;
        players.id[i].cards_num = 0;
    }
    
    //setSekijun();//席順ベクトルを作成する
    //setMibun();//身分ベクトルを形成する
    result->setup( &players );//プレイヤー情報で更新する
    
    //最初の手札の決定
    dealCards();//プレイヤたちにカードを分配する
    if( players.isMisalliance() ){//カード交換（搾取）
        //cout << "CC" << endl;
        //最初の手札通知では、大富豪・富豪には"搾取後"の手札が通知される。
        //一方で大貧民・貧民には"搾取前"の手札が通知される。
        int DHtoDF[8][15]={{0}};//大貧民から大富豪へ
        int HtoF[8][15]={{0}};//貧民から富豪へ
        selectHighCard(DHtoDF, players.id[players.mibunId(DAIHINMIN)].cards, 2);//2枚選び
        result->addChange(players.mibunId(DAIHINMIN), players.mibunId(DAIFUGO), DHtoDF);//大貧民の交換カードを保存
        selectHighCard(HtoF, players.id[players.mibunId(HINMIN)].cards, 1);//1枚選び
        result->addChange(players.mibunId(HINMIN), players.mibunId(FUGO), DHtoDF);//貧民の交換カードを保存
        addCard(players.id[players.mibunId(DAIFUGO)].cards, DHtoDF);//足す
        addCard(players.id[players.mibunId(FUGO)].cards, HtoF);//足す
    }
    
    //最初のプレイヤーIDをturnに保存する
    players.turn = whoHave(0, 1);//ダイヤの3を持つものが最初のターンである
    //ここで3を持ってる人を知らせては適当でないのでは？
    
    //最初の手札を送付する
    int tefuda[5][8][15]={{0}};
    makeTefuda815(tefuda);
    for(int i=0;i<config.PLAYER_NUM;i++){
        tefuda[i][5][0] = 1;//最初なのでカード交換
        send815(tefuda[i], players.id[i].sockfd );
    }
    
}

void Game::start( Result *result){
    //各プレイヤーの順位が決定するまでゲームを行う
    COUT<<"start"<<endl;
    
    if( players.isMisalliance() ){
        changeCardsUEC( result );//カードの交換
    }
    
    players.setCardsNum();
    
    while( !isGameEnd() ){//試合が存続中である（順位が決していない）

        {//各プレイヤの手札を作り、送る
            int sendTefuda[config.PLAYER_NUM][8][15]={{0}};
            makeTefuda815( sendTefuda );
            for(int i=0; i<config.PLAYER_NUM; i++){
                send815( sendTefuda[i], players.id[i].sockfd );
            }
        }
        
        int data[8][15]={{0}};
        recv815(data, players.id[players.turnId()].sockfd);//現在のターンのプレイヤから提出カードを受け取る
        
        //players.print();
        //table.print();
        PRINT815(players.id[players.turnId()].cards);
        COUT << " get " << endl;
        PRINT815(data);
        //提出されたデータの解析
        Yaku yaku;//受け取ったデータの変換先
        int num = 0;//返信用番号
        if( conv815toYaku(&yaku, data) ){//815はそもそも"役"として解釈可能か？
            //解釈可能ならばyaku形式に変換されている
            if(isSubmittableYaku(yaku)){//提出可能な手か？
                if(yaku.isPass()){//パスと明示している（UEC標準ルールには存在しない）
                    if( config.PROTOCOL!=20070 ){//
                        num = 7;
                    }else{
                        num = 8;
                    }
                }else{//提出可能な役
                    num = 9;
                }
            }else{//提出不可能な役
                //パスとして更新する
                yaku.demoPass();
                num = 8;
            }
        }else{//役として成り立っていない
            //cout << "unable to convert into Yaku!" << endl;
            //print815( data );
            //yaku.printBit();
            
            yaku.demoPass();
            num = 8;
        }
        //cout << " conv " << endl;
        //yaku.print();
        
        //役の保存
        Table bTable = table;//提出前の盤面
        vector<int> passArray, numArray;//パスしてるかの配列
        makePassArray( &passArray );//パスアレイを作る
        makeNumArray( &numArray );//枚数アレイを作る
        result->addAct( players.turn, bTable, passArray, numArray, yaku );//提出役の記録
        
        //result->print();
        //cout << "n" << endl;
        //以下、実際の盤面更新
        switch( num ){
        case 7:
            COUT << 7 << endl;
            send1( 7, players.id[players.turnId()].sockfd );//パスとしてパスをした
            pass();//パスして
            break;
        case 8:
            COUT << 8 << endl;
            send1( 8, players.id[players.turnId()].sockfd );//パスしたとみなした
            pass();
            break;
        case 9:
            COUT << 9 << endl;
            //役による盤面更新
            send1( 9, players.id[players.turnId()].sockfd );//提出できた
            //players[convSekiToPID(turn)].cards_num -= yaku.mNum;
            update(yaku);//プレイヤー情報の更新
            break;
        }
        //cout << "ee" << endl;
        {//更新された場札を送る
            int sendBafuda[config.PLAYER_NUM][8][15];
            makeBafuda815( sendBafuda );//加工
            //cout << " 90 k " << endl;
            //print815( sendBafuda[0] );
            for(int i=0; i<config.PLAYER_NUM; i++){
                send815( sendBafuda[i], players.id[i].sockfd );
            }
        }
        
        {
            if(isGameEnd()){
                //cout << "GE" << endl;
                //試合が終わってしまった
                result->mAgari = players.agari;//あがった順番をresultに保存する
                for(int i=0; i<players.agari.size(); i++){
                    players.id[players.agari[i]].mibun = i;
                }
                result->finish( players );
                break;//おさらば
            }else{
                //まだ終わってない
                for(int i=0; i<config.PLAYER_NUM; i++){
                    send1( 0, players.id[i].sockfd );
                }
            }
        }
        /*
        cout << " cards_num " << endl;
        for(int i=0; i<5; i++){
            cout << players.id[i].cards_num << endl;
        }
        */
    }//while
    
}

void Game::changeCardsUEC( Result *result ){
    //UECdaルールによる手札の交換
    COUT<<"change"<<endl;
    int temp_card[8][15];
    make815Table(temp_card);//テーブルの情報をカードに書き込む
    make815Player(temp_card);//プレイヤーの情報をカードに書き込む
    
    //Gameコンストラクタですでに搾取されている
    
    //交換するカードを受け取り、確認を行う（通信）
    //大富豪から大貧民へ
    int data[8][15] = {{0}};
    recv815(data, players.id[players.mibunId(DAIFUGO)].sockfd);
    if( !checkChangeCardUEC(data, players.id[players.mibunId(DAIFUGO)]) ){//
        //ダメなら、UECサーバは勝手に搾取してしまうようだ
        memset(data, 0, sizeof(data));
        selectHighCard(data, players.id[players.mibunId(DAIFUGO)].cards, 2);
    }
    result->addChange(players.mibunId(DAIFUGO), players.mibunId(DAIHINMIN), data);//大富豪の交換カードを保存
    deleteCard(players.id[players.mibunId(DAIFUGO)].cards, data);//交換できるなら削除して
    addCard(players.id[players.mibunId(DAIHINMIN)].cards, data);//押し付ける

    //富豪から貧民へ
    memset(data, 0, sizeof(data));
    recv815(data, players.id[players.mibunId(FUGO)].sockfd);
    if( !checkChangeCardUEC(data, players.id[players.mibunId(FUGO)]) ){//
        //ダメなら、UECサーバは勝手に搾取してしまうようだ
        memset(data, 0, sizeof(data));
        selectHighCard(data, players.id[players.mibunId(FUGO)].cards, 1);
    }
    result->addChange(players.mibunId(FUGO), players.mibunId(HINMIN), data);//富豪の交換カードを保存
    deleteCard(players.id[players.mibunId(FUGO)].cards, data);//交換できるなら削除して
    addCard(players.id[players.mibunId(HINMIN)].cards, data);//押し付ける
    
    //コンストラクタ時点で足されていたものを修復する
    for(int i=0; i<result->mChange.size(); i++){
        if(result->mChange[i].mFrom == players.mibunId(HINMIN)){
            deleteCard(players.id[players.mibunId(HINMIN)].cards, result->mChange[i].mCards);
        }else if(result->mChange[i].mFrom == players.mibunId(DAIHINMIN)){
            deleteCard(players.id[players.mibunId(DAIHINMIN)].cards, result->mChange[i].mCards);
        }
    }
}
/*
void Game::setMibun(){
    for(int i=0;i<5;i++){//身分番号
        for(int j=0;j<players.size();j++){
            if(players[j].mibun == i){
                mibun.push_back( j );
            }
        }
    }
}

void Game::setSekijun(){
    for(int i=0; i<config.PLAYER_NUM; i++){
        for(int j=0; j<config.PLAYER_NUM; j++){
            if(players[j].sekijun == i){
                sekijun.push_back(j);
                break;
            }
        }
    }
}
*/

void Game::make815Table(int card[8][15]){
    COUT<<"m8t"<<endl;
    //card[5][0]=;//カード交換中である
    //card[5][1]=;//カード交換の枚数
    //card[5][2]=;//自分のターンか
    card[5][3] = players.turn;//何番目のターンか
    card[5][4] = players.turnId();//ターンのプレイヤー番号
    card[5][5] = table.isOnset();
    card[5][6] = table.is11back();
    card[5][7] = table.isKakumei();
    card[5][8] = table.isShibari();
}

void Game::make815Player(int card[8][15]){
    COUT<<"m8p"<<endl;
    //手札の枚数
    for(int i=0; i<players.size(); i++){
        card[6][0+i] = players.id[i].cards_num;
    }
    //身分
    for(int i=0; i<players.size(); i++){
        card[6][5+i] = players.id[i].mibun;
    }
    //席順
    for(int i=0; i<players.size(); i++){
        card[6][10+i] = players.sekijun[i];
    }
}

int Game::whoHave(int suit, int rank){
    COUT<<"wh"<<endl;
    for(int i=0; i<config.PLAYER_NUM; i++){
        if(players.id[i].cards[suit][rank]!=0){
            return i;
        }
    }
    return -1;
}

bool Game::isCardChangePhase(){
    COUT<<"is"<<endl;
    if(players.id[0].mibun != HEIMIN || players.id[1].mibun != HEIMIN){//プレイヤーに身分差がついていれば
        return true;
    }else{
        return false;
    }
}

void Game::makePassArray( vector<int> *array ){
    COUT<<"mpa"<<endl;
    for(int i=0;i<config.PLAYER_NUM;i++){
        array->push_back( players.id[players.sekijun[i]].passed );
    }
}

void Game::makeNumArray( vector<int> *array ){
    COUT<<"mna"<<endl;
    for(int i=0;i<config.PLAYER_NUM;i++){
        array->push_back( players.id[players.sekijun[i]].cards_num );
    }
}

bool Game::isSubmittableYaku(const Yaku &yaku){
    COUT<<"isty"<<endl;
    //提出カードが上に出せるのか
    if( table.isShibari() && (table.mSuits!=yaku.mSuits) ){
        COUT << "" << endl;
        return false;
    }
    
    if(yaku.isPass()){//そもそもパスする気でいる
        COUT << "pas" << endl;
        return false;
    }else if(table.isKaidan()){
        COUT << "kdn" << endl;
        if(yaku.isKaidan() && yaku.mNum==table.mNum){
            if(table.isKakumei()){
                if(table.mRankL > yaku.mRankR){
                    return true;
                }
            }else{
                if(table.mRankR < yaku.mRankL){
                    return true;
                }
            }
        }
    }else if(table.isPair()){
        COUT << "pir" << endl;
        if(yaku.isPair() && yaku.mNum==table.mNum){
            if(table.isKakumei()){
                if(table.mRankL > yaku.mRankL){
                    return true;
                }
            }else{
                if(table.mRankL < yaku.mRankL){
                    return true;
                }
            }
        }
    }else if(table.isTanki()){
        COUT << "tnk" << endl;
        if(yaku.isTanki()){
            if(table.isJTanki() && yaku.isSpade3()){
                return true;
            }else if(yaku.isJTanki()){
                return true;
            }else if(table.isKakumei()){
                if(table.mRankL > yaku.mRankL){
                    return true;
                }
            }else{
                if(table.mRankL < yaku.mRankL){
                    return true;
                }
            }
        }
    }else if(table.isOnset()){
        COUT << "ons" << endl;
        return true;
    }
    return false;
}

void Game::update(const Yaku &yaku){//役での更新
    COUT<<"up"<<endl;
    //yaku.print();
    //ここに来るということはyakuにより更新されることは決まっている。
    //のでyakuの情報で上書きしていけば良い
    
    int card[8][15]={{0}};
    //cout << " a" << endl;
    yaku.setBitTo815(card);
    players.id[players.turnId()].subtraction(card);//提出カードを差し引く
    players.id[players.turnId()].cards_num -= yaku.mNum;//枚数も差し引く
    if(players.id[players.turnId()].cards_num<0){
        yaku.print();
        exit(1);
    
    }
    if( players.id[players.turnId()].cards_num == 0 ){//あがった
        players.addAgari( players.turnId() );
    }
    //cout << " b" << endl;
    if( yaku.is8giri() || (table.isJTanki() && yaku.isSpade3()) ){//場が流れる
        purge();//流す
        //turnはそのまま
    }
    else{
        //cout << "c" << endl;
        table.mBafuda = yaku;
        table.setOnset(false);
        table.mRankL = yaku.mRankL;
        table.mRankR = yaku.mRankR;
        table.mNum = yaku.mNum;
        if(table.mSuits == yaku.mSuits){
            table.setShibari(1);
        }else{
            table.mSuits = yaku.mSuits;
        }
        if(yaku.isKaidan()){
            table.setKaidan(true);
        }else if(yaku.isPair()){
            table.setPair(true);
        }else if(yaku.isTanki()){
            table.setTanki(true);
        }
        //cout << "d" << endl;
        players.nextTurn();
    }
    //cout << "f" << endl;
}

/*
void Game::nextTurn(){
    turn = (turn + 1) % sekijun.size();
    while( players[convSekiToPID(turn)].passed == true || players[convSekiToPID(turn)].cards_num==0 ){
        turn = (turn + 1) % sekijun.size();
    }
}
*/

void Game::purge(){//場を流す
    COUT<<"pur"<<endl;
    //場札情報を初期化する
    table.purge();
    
    //パスから復帰させる
    players.recover();
    
    //turnは最後にパスした人（つまり現在の人）なので変更しない
    //が、あがっていればその限りでない
    if( players.id[players.turnId()].cards_num == 0){
        players.nextTurn();
    }
}

bool Game::conv815toYaku(Yaku *yaku, int data[8][15]){//
    COUT<<"c8ty"<<endl;
    //提出役が本当に提出可能かどうかを確認し、yakuに変換する
    int cards_num = 0, joker_num = 0;
    vector<int> suit, rank, fake;//発見されたカードをいれていく
    for(int i=0; i<8; i++){
        for(int j=0; j<15; j++){
            if(data[i][j]==0){//提出されていない
                
            }else if(data[i][j]==1){//通常のカードが提出された
                suit.push_back(i);
                rank.push_back(j);
                fake.push_back(0);
                cards_num++;
            }else if(data[i][j]==2){//jokerが提出された
                suit.push_back(i);
                rank.push_back(j);
                fake.push_back(1);
                cards_num++;
                joker_num++;
            }else{//変な数字が入っている
                return false;
            }
        }
    }
    
    //joker枚数の確認
    if(joker_num > config.JOKER_NUM){//そもそもルール規定より多い
        return false;
    }
    //ほんとにjokerを持っているのか？
    for(int i=0; i<joker_num; i++){
        if(players.id[players.turnId()].cards[4][1 + i]==0){
            return false;
        }
    }
    //ほんとにカードを持っているのか？
    for(int i=0; i<cards_num; i++){
        //jokerを使ったカード以外のものについて
        if(fake[i]==0 && players.id[players.turnId()].cards[suit[i]][rank[i]]==0){
            return false;
        }
    }
    //ならばどのタイプの可能性があるか？
    bool tanki_flag=true, kaidan_flag=true, pair_flag=true;
    if(cards_num==0){
        tanki_flag=false;
        kaidan_flag=false;
        pair_flag=false;
    }else if(cards_num==1){
        tanki_flag=true;
        kaidan_flag=false;
        pair_flag=false;
    }else{
        tanki_flag=false;
        for(int i=0; i<cards_num-1; i++){
            //1箇所でも例外があればfalseとなる
            if(suit[i]!=suit[i+1]){//suitが異なる
                kaidan_flag=false;
            }
            if(rank[i]!=rank[i+1]){//rankが異なる
                pair_flag=false;
            }
        }
    }
    
    if(tanki_flag){//それは単騎である
        yaku->set815ToBit( data );
        yaku->mNum = cards_num;
        if(joker_num == 1){//joker単騎
            yaku->mRankR = 14;
            yaku->mRankL = 0;
            yaku->setSuit(0);
            yaku->setJTanki();
        }else{
            yaku->mRankR = rank[0];
            yaku->mRankL = rank[0];
            yaku->setSuit(0);
        }
        yaku->setTanki();
    }else if(pair_flag){//それはペアである
        yaku->set815ToBit( data );
        yaku->mNum = cards_num;
        yaku->mRankR = rank[0];
        yaku->mRankL = rank[0];
        for(int i=0; i<suit.size(); i++){
            yaku->setSuit(suit[i]);
        }
        yaku->setPair();
    }else if(kaidan_flag){//それは階段である
        yaku->set815ToBit( data );
        yaku->mNum = cards_num;
        yaku->mRankL = rank[0];//最初に見つかるものが始点に決まっている
        yaku->mRankR = rank[0]+cards_num-1;
        yaku->setSuit(suit[0]);
        yaku->setKaidan();
    }else{//非受理
        return false;
    }
    
    //この長い審査を通った時ようやくtrueが蛙
    return true;
}

void Game::dealCards(){
    COUT<<"dc"<<endl;
    //カードを配る
    
    vector<int> deck;
    
    //通常のカード
    for(int i=0; i<4; i++){
        for(int j=1; j<=13; j++){
            deck.push_back( i*14 + j );
        }
    }
    //jokerの枚数
    for(int i=0; i<config.JOKER_NUM; i++){
        deck.push_back( 14*4 + 1 );
    }
    
    //deckをshuffle
    for(int i=0; i<deck.size(); i++){
        int a = (int)( (deck.size()-1) * ((double)random()/RAND_MAX));
        int b = (int)( (deck.size()-1) * ((double)random()/RAND_MAX));
        int c = deck[a];
        deck[a] = deck[b];
        deck[b] = c;
    }
    
    //プレイヤのカードを初期化してから
    for(int i=0; i<players.size(); i++){
        players.id[i].initCard();
    }
    //deckを一枚ずつ分配していく
    int target = 0;
    while( !deck.empty() ){
        players.id[target].cards[deck[0]/14][deck[0]%14] = 1;//入れて
        players.id[target].cards_num++;//枚数を増やして
        deck.erase( deck.begin() );//削除
        
        //次の人
        target = ( target + 1 ) % ( players.size() );
    }
}

bool checkChangeCardUEC(int data[8][15], Player &player){
    COUT<<"ccc"<<endl;
    //交換可能なカードなのか？
    int cnt = 0;
    bool gacchi = true;
    
    for(int i=0; i<5; i++){
        for(int j=0; j<15; j++){
            if(data[i][j]!=0){
                cnt++;
                if( player.cards[i][j]==0 ){//持っていなかったら
                    gacchi = false;
                }
            }
        }
    }
    if( player.mibun == FUGO ){//富豪なら1枚
        if( cnt == 1 && gacchi ){
            return true;
        }else{
            return false;
        }
    }else if( player.mibun == DAIFUGO ){//大富豪なら2枚
        if( cnt == 2 && gacchi ){
            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }
}

/*
int Game::convSekiToPID(int idx){
    //idx番の席に座っているプレイヤのIDを返す
    return sekijun[idx];
}
*/

void Game::makeTefuda815(int data[5][8][15]){
    COUT<<"mt8"<<endl;
    //UECda形式の、送信する815形式の配列を作成する
    for(int i=0; i<5; i++){//プレイヤー
        for(int j=0; j<8; j++){//スート
            for(int k=0; k<15; k++){//ランク
                data[i][j][k] = 0;
            }
        }
    }
    //カード部
    for(int i=0; i<5; i++){//プレイヤー
        for(int j=0; j<4; j++){//スート
            for(int k=1; k<=13; k++){//ランク
                if(players.id[i].cards[j][k]!=0){
                    data[i][j][k] = 1;
                }
            }
        }        
        //ジョーカー
        for(int j=0; j<config.JOKER_NUM; j++){
            if(players.id[i].cards[4][1+j]!=0){
                data[i][4][1+j] = 2;
            }
        }
    }
    
    //盤面部
    int temp[8][15]={{0}};
    //交換フェーズか
    if(table.isChangePhase()){
        temp[5][0] = 1;
    }
    
    //場が流れたか
    if(table.isOnset()){
        temp[5][4] = 1;
    }
    //イレブンバック
    if(table.is11back()){
        temp[5][5] = 1;
    }
    //革命
    if(table.isKakumei()){
        temp[5][6] = 1;
    }
    //しばり
    if(table.isShibari()){
        temp[5][7] = 1;
    }
    
    //プレイヤー部分
    for(int i=0;i<5;i++){
        //手札の枚数
        temp[6][0+i] = players.id[i].cards_num;
        //階級
        temp[6][5+i] = players.id[i].mibun;
        //席順
        temp[6][10+i] = players.sekijun[i];
        
        //現在のターンのプレイヤー番号
        //temp[5][3] = convSekiToPID(turn);
        temp[5][3] = players.turnId();
    }
    
    //共通部分をコピー
    for(int i=0;i<5;i++){
        for(int j=5;j<8;j++){
            for(int k=0;k<15;k++){
                data[i][j][k] = temp[j][k];
            }
        }
    }
    
    //各プレイヤーによって違うもの
    for(int i=0;i<5;i++){
        //交換枚数
        if(players.id[i].mibun == DAIFUGO){
            data[i][5][1] = 2;
        }else if(players.id[i].mibun == FUGO){
            data[i][5][1] = 1;
        }
        //汝のターンか
        /*
        if(convSekiToPID(turn) == players[i].id){
            data[i][5][2] = 1;
        }
        */
    }
    data[players.turnId()][5][2] = 1;
}

void Game::makeBafuda815(int data[5][8][15]){
    COUT<<"mb8"<<endl;
    //UECda形式の、送信する815形式の配列を作成する
    for(int i=0; i<5; i++){//プレイヤー
        for(int j=0; j<8; j++){//スート
            for(int k=0; k<15; k++){//ランク
                data[i][j][k] = 0;
            }
        }
    }
    //カード部
    int bafuda[8][15] = {{0}};
    table.mBafuda.setBitTo815(bafuda);
    for(int i=0; i<5; i++){//プレイヤー
        for(int j=0; j<5; j++){//スート
            for(int k=0; k<15; k++){
                data[i][j][k] = bafuda[j][k];
            }
        }
    }
    
    //盤面部
    int temp[8][15]={{0}};
    //交換フェーズか
    if(table.isChangePhase()){
        temp[5][0] = 1;
    }
    
    //場が流れたか
    if(table.isOnset()){
        temp[5][4] = 1;
    }
    //イレブンバック
    if(table.is11back()){
        temp[5][5] = 1;
    }
    //革命
    if(table.isKakumei()){
        temp[5][6] = 1;
    }
    //しばり
    if(table.isShibari()){
        temp[5][7] = 1;
    }
    
    //プレイヤー部分
    for(int i=0;i<5;i++){
        //手札の枚数
        temp[6][0+i] = players.id[i].cards_num;
        //階級
        temp[6][5+i] = players.id[i].mibun;
        //席順
        temp[6][10+i] = players.sekijun[i];
        
        //現在のターンのプレイヤー番号
        //temp[5][3] = convSekiToPID(turn);
        temp[5][3] = players.turnId();
    }
    
    //共通部分をコピー
    for(int i=0;i<5;i++){
        for(int j=5;j<8;j++){
            for(int k=0;k<15;k++){
                data[i][j][k] = temp[j][k];
            }
        }
    }
    
    //各プレイヤーによって違うもの
    for(int i=0;i<5;i++){
        //交換枚数
        if(players.id[i].mibun == DAIFUGO){
            data[i][5][1] = 2;
        }else if(players.id[i].mibun == FUGO){
            data[i][5][1] = 1;
        }
        //汝のターンか
        /*
        if(convSekiToPID(turn) == players[i].id){
            data[i][5][2] = 1;
        }
        */
    }
    data[players.turnId()][5][2] = 1;
}

bool Game::isGameEnd(){
    COUT<<"ige"<<endl;
    //ゲームは終わったか？
    int agari_num = 0;
    int agattenai = 0;
    for(int i=0; i<config.PLAYER_NUM; i++){
        if(players.id[i].cards_num == 0){
            agari_num++;
        }else{
            agattenai = i;//最後の上がってない人
        }
    }
    
    if(agari_num==config.PLAYER_NUM || agari_num==(config.PLAYER_NUM-1)){
        players.addAgari( agattenai );
        return true;
    }else{
        return false;
    }
}

/*
void Game::sekigae(){
    int a = (int)( (players.size()-1) * ((double)random()/RAND_MAX));
    int b = (int)( (players.size()-1) * ((double)random()/RAND_MAX));
    int c = players.sekijun[a];
    players.sekijun[a] = players.sekijun[b];
    players.sekijun[b] = c;
}

void Game::reset(){
    sekigae();
    for(int i=0; i<players.size(); i++){
        players.mibun[i] = HEIMIN;
    }
}
*/

void Game::pass(){
    COUT<<"pass"<<endl;
    players.id[players.turnId()].passed = true;
    for(int i=0;i<config.PLAYER_NUM;i++){
        if(players.id[i].passed == false && players.id[i].cards_num!=0 ){//あがってなくてかつパスしていない人がいるなら
            players.nextTurn();//次の人
            return;
        }
    }
    //全員パスしている
    purge();
}

void Game::recover(){
    COUT<<"rec"<<endl;
    for(int i=0;i<players.size();i++){
        players.id[i].passed = false;
    }
}

