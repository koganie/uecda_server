#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<iostream>
#include "Game.h"
//#include "makeYaku.h"
#include "common.h"
#include "connection.h"

#include "debug.h"

/*
Game::Game(const Configure &c, const Players &p, Result *result){
    //GAMEコンストラクタ
    //最初の手札を配分して、通知までする
    COUT<<"GAME"<<endl;
    
    config = c;//ゲームのルールを格納する
    players = p;//プレイヤーをコピーする
    
    for( int i=0; i<5; i++){
        players.id[i].cards_num = 0;
    }
    
    result->setup( &players, config );//プレイヤー情報で更新する
    
    //最初の手札の決定
    dealCards();//プレイヤたちにカードを分配する
    
    if( players.isMisalliance() ){//カード交換（搾取）
        //最初の手札通知では、大富豪・富豪には"搾取後"の手札が通知される。
        //一方で大貧民・貧民には"搾取前"の手札が通知される。
        int data[8][15] = {{0}};
        //大貧民から大富豪に
        selectHighCard(data, players.id[players.mibunId(DAIHINMIN)].cards, 2);//2枚選び
        result->addChange(players.mibunId(DAIHINMIN), players.mibunId(DAIFUGO), data);//大貧民の交換カードを保存
        addCard(players.id[players.mibunId(DAIFUGO)].cards, data);//足す
        memset(data, 0, sizeof(data));
        //貧民から富豪に
        selectHighCard(data, players.id[players.mibunId(HINMIN)].cards, 1);//1枚選び
        result->addChange(players.mibunId(HINMIN), players.mibunId(FUGO), data);//貧民の交換カードを保存
        addCard(players.id[players.mibunId(FUGO)].cards, data);//足す
        
    }
    
    players.turn = -1;//交換時にはターンの人が変わる可能性があるのでここでは入れない
    
    //最初の手札を送付する
    int tefuda[5][8][15]={{0}};
    makeTefuda815(tefuda);
    for(int i=0;i<config.PLAYER_NUM;i++){
        tefuda[i][5][0] = 1;//最初なのでカード交換
        switch(players.id[i].mibun){//身分ごとに交換カード枚数を格納する
        case DAIFUGO:
            tefuda[i][5][1] = 2;
            break;
        case FUGO:
            tefuda[i][5][1] = 1;
            break;
        case HEIMIN:
            tefuda[i][5][1] = 0;
            break;
        case HINMIN:
            tefuda[i][5][1] = -1;
            break;
        case DAIHINMIN:
            tefuda[i][5][1] = -2;
            break;
        }
        
        send815(tefuda[i], players.id[i].sockfd );//それぞれに通知
    }
    
    result->setFirstCards( tefuda );
}
*/

Game::Game(const Configure &c, const Players &p, Result *result){
    //GAMEコンストラクタ
    //最初の手札を配分して、通知までする
    COUT<<"GAME"<<endl;
    
    config = c;//ゲームのルールを格納する
    players = p;//プレイヤーをコピーする
    
    for( int i=0; i<5; i++){
        players.id[i].cards_num = 0;
    }
    
    result->setup( &players, config );//プレイヤー情報で更新する
    
    /*
    //最初の手札の決定
    dealCards();//プレイヤたちにカードを分配する
    
    if( players.isMisalliance() ){//カード交換（搾取）
        //最初の手札通知では、大富豪・富豪には"搾取後"の手札が通知される。
        //一方で大貧民・貧民には"搾取前"の手札が通知される。
        int data[8][15] = {{0}};
        //大貧民から大富豪に
        selectHighCard(data, players.id[players.mibunId(DAIHINMIN)].cards, 2);//2枚選び
        result->addChange(players.mibunId(DAIHINMIN), players.mibunId(DAIFUGO), data);//大貧民の交換カードを保存
        addCard(players.id[players.mibunId(DAIFUGO)].cards, data);//足す
        memset(data, 0, sizeof(data));
        //貧民から富豪に
        selectHighCard(data, players.id[players.mibunId(HINMIN)].cards, 1);//1枚選び
        result->addChange(players.mibunId(HINMIN), players.mibunId(FUGO), data);//貧民の交換カードを保存
        addCard(players.id[players.mibunId(FUGO)].cards, data);//足す
        
    }
    */
    
    //dealCards( result );
    /*
    players.turn = -1;//交換時にはターンの人が変わる可能性があるのでここでは入れない
    
    //最初の手札を送付する
    int tefuda[5][8][15]={{0}};
    makeTefuda815(tefuda);
    for(int i=0;i<config.PLAYER_NUM;i++){
        tefuda[i][5][0] = 1;//最初なのでカード交換
        switch(players.id[i].mibun){//身分ごとに交換カード枚数を格納する
        case DAIFUGO:
            tefuda[i][5][1] = 2;
            break;
        case FUGO:
            tefuda[i][5][1] = 1;
            break;
        case HEIMIN:
            tefuda[i][5][1] = 0;
            break;
        case HINMIN:
            tefuda[i][5][1] = -1;
            break;
        case DAIHINMIN:
            tefuda[i][5][1] = -2;
            break;
        }
        
        send815(tefuda[i], players.id[i].sockfd );//それぞれに通知
    }
    
    result->setFirstCards( tefuda );
    */
}

void Game::sendFirstCards( Result *result ){
    players.turn = -1;//交換時にはターンの人が変わる可能性があるのでここでは入れない
    
    //最初の手札を送付する
    int tefuda[5][8][15]={{0}};
    makeTefuda815(tefuda);
    for(int i=0;i<config.PLAYER_NUM;i++){
        tefuda[i][5][0] = 1;//最初なのでカード交換
        switch(players.id[i].mibun){//身分ごとに交換カード枚数を格納する
        case DAIFUGO:
            tefuda[i][5][1] = 2;
            break;
        case FUGO:
            tefuda[i][5][1] = 1;
            break;
        case HEIMIN:
            tefuda[i][5][1] = 0;
            break;
        case HINMIN:
            tefuda[i][5][1] = -1;
            break;
        case DAIHINMIN:
            tefuda[i][5][1] = -2;
            break;
        }
        
        send815(tefuda[i], players.id[i].sockfd );//それぞれに通知
    }
    
    result->setFirstCards( tefuda );
}

void Game::start( Result *result){
    //各プレイヤーの順位が決定するまでゲームを行う
    COUT<<"start"<<endl;
    
    if( players.isMisalliance() ){//身分差があるなら
        changeCardsUEC( result );//カードの交換
    }
    
    players.setCardsNum();//プレイヤーのカード枚数を確認する
    
    //最初のプレイヤーIDをturnに保存する
    players.turn = players.convIDtoSekiNum(whoHave(2, 1));//ダイヤの3を持つものが最初のターンである
    
    while( !isGameEnd() ){//試合が存続中である（順位がすべて決していない）

        {//各プレイヤの手札を作り、送る
            int sendTefuda[config.PLAYER_NUM][8][15]={{0}};
            makeTefuda815( sendTefuda );
            for(int i=0; i<config.PLAYER_NUM; i++){
                send815( sendTefuda[i], players.id[i].sockfd );
            }
            COUT<<"send hand"<<endl;
            PRINT815( sendTefuda[players.turnId()] );
        }
        int data[8][15]={{0}};
        recv815(data, players.id[players.turnId()].sockfd);//現在のターンのプレイヤから提出カードを受け取る
        
        #ifdef DEBUGMODE
        //デバグ用出力
        table.print();
        players.print();
        COUT<<"get"<<endl;
        PRINT815(data);
        #endif
        
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
            //UECルールではパス（出せる手がない場合）もここにくるので出力させているときりがない
            //table.print();
            //print815( data );
            //yaku.printBit();
            yaku.demoPass();
            num = 8;
        }
        
        {//提出手役の記録
            Table bTable = table;//提出前の盤面
            vector<int> passArray, numArray;//パスしてるかの配列
            makePassArray( &passArray );//パスアレイを作る
            makeNumArray( &numArray );//枚数アレイを作る
            result->addAct( players.turn, bTable, passArray, numArray, yaku );//提出役の記録
            //result->print();
        }
        
        table.player_id = players.turnId();
        
        //以下、実際の盤面更新
        switch( num ){
        case 7:
            COUT << 7 << endl;
            send1( 7, players.id[players.turnId()].sockfd );//パスとしてパスをした
            pass();
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
            update(yaku);//役情報による盤面の更新
            break;
        }
        
        {//更新された場札を送る
            int sendBafuda[config.PLAYER_NUM][8][15] = {{0}};
            makeBafuda815( sendBafuda );//加工
            for(int i=0; i<5; i++){
                send815( sendBafuda[i], players.id[i].sockfd );
            }
            COUT<<"send bafuda"<<endl;
            PRINT815( sendBafuda[players.turnId()] );
        }
            
        //場札を通知したら、次のプレイヤーに移るが、場が流れることもある
        bool pf  = true;
        switch(num){
        case 7:
        case 8:
            //パスした場合の次のプレイヤー
            if( isBaAlive() ){
                //まだ回せるなら回す
                players.nextTurn();
            }else{
                //場を流す
                purge();
            }
            break;
        case 9:
            //何かが通知されれば次のプレイヤーに移るが、場が流れることもある
            if( table.isOnset() ){
                purge();
                break;
            }
            players.nextTurn();
            break;
        }
            
        {
            //試合終了の判定
            if(isGameEnd()){
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
        
    }//while
    
}

void Game::changeCardsUEC( Result *result ){
    //UECdaルールによる手札の交換
    COUT<<"change"<<endl;
    
    //Gameコンストラクタですでに搾取されている
    //コンストラクタ時点で足されていたものを修復する
    for(int i=0; i<result->mChange.size(); i++){
        if(result->mChange[i].mFrom == players.mibunId(HINMIN)){
            deleteCard(players.id[players.mibunId(HINMIN)].cards, result->mChange[i].mCards);
        }else if(result->mChange[i].mFrom == players.mibunId(DAIHINMIN)){
            deleteCard(players.id[players.mibunId(DAIHINMIN)].cards, result->mChange[i].mCards);
        }
    }
    
    //交換するカードを受け取り、確認を行う（通信）
    //大富豪から大貧民へ
    int data[8][15] = {{0}};
    recv815(data, players.id[players.mibunId(DAIFUGO)].sockfd);
    if( !checkChangeCardUEC(data, players.id[players.mibunId(DAIFUGO)]) ){//
        cout << " DAIFUGO change faled " << endl;
        //ダメなら、UECサーバは勝手に搾取してしまうようだ
        memset(data, 0, sizeof(data));
        selectHighCard(data, players.id[players.mibunId(DAIFUGO)].cards, 2);
    }
    //PRINT815(data);
    result->addChange(players.mibunId(DAIFUGO), players.mibunId(DAIHINMIN), data);//大富豪の交換カードを保存
    deleteCard(players.id[players.mibunId(DAIFUGO)].cards, data);//交換できるなら削除して
    addCard(players.id[players.mibunId(DAIHINMIN)].cards, data);//押し付ける

    //富豪から貧民へ
    memset(data, 0, sizeof(data));
    recv815(data, players.id[players.mibunId(FUGO)].sockfd);
    if( !checkChangeCardUEC(data, players.id[players.mibunId(FUGO)]) ){//
        cout << " FUGO change faled " << endl;
        //ダメなら、UECサーバは勝手に搾取してしまうようだ
        memset(data, 0, sizeof(data));
        selectHighCard(data, players.id[players.mibunId(FUGO)].cards, 1);
    }
    //PRINT815(data);
    result->addChange(players.mibunId(FUGO), players.mibunId(HINMIN), data);//富豪の交換カードを保存
    deleteCard(players.id[players.mibunId(FUGO)].cards, data);//交換できるなら削除して
    addCard(players.id[players.mibunId(HINMIN)].cards, data);//押し付ける
    
    if(0){
        COUT<<"CHANGE"<<endl;
        for(int i=0; i<result->mChange.size(); i++){
            result->mChange[i].print();
        }
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

/*
bool Game::isCardChangePhase(){
    COUT<<"is"<<endl;
    if(players.id[0].mibun != HEIMIN || players.id[1].mibun != HEIMIN){//プレイヤーに身分差がついていれば
        return true;
    }else{
        return false;
    }
}
*/

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
    
    //場が縛りで出すスートが異なり、ジョーカー単騎でもない
    if( table.isShibari() && (table.mBafuda.mSuits!=yaku.mSuits) && !yaku.isJTanki() ){
        COUT << "" << endl;
        return false;
    }
    
    if(yaku.isPass()){//そもそもパスする気でいる
        COUT << "pas" << endl;
        return false;
    }else if(table.isKaidan()){
        COUT << "kdn" << endl;
        if(yaku.isKaidan() && yaku.mNum==table.mBafuda.mNum){
            if(table.isKakumei()){
                if(table.mBafuda.mRankL > yaku.mRankR){
                    return true;
                }
            }else{
                if(table.mBafuda.mRankR < yaku.mRankL){
                    return true;
                }
            }
        }
    }else if(table.isPair()){
        COUT << "pir" << endl;
        if(yaku.isPair() && yaku.mNum==table.mBafuda.mNum){
            if(table.isKakumei()){
                if(table.mBafuda.mRankL > yaku.mRankL){
                    return true;
                }
            }else{
                if(table.mBafuda.mRankL < yaku.mRankL){
                    return true;
                }
            }
        }
    }else if(table.isTanki()){
        COUT << "tnk" << endl;
        if(yaku.isTanki()){
            //cout << config.RULE_SPADE3 <<" " << table.mBafuda.isJTanki() <<" " << yaku.isSpade3() << endl;
            if( config.RULE_SPADE3 && table.mBafuda.isJTanki() && yaku.isSpade3()){
                //cout<<"nemutai nemutai nemutai"<<endl;
                return true;
            }else if(yaku.isJTanki()){
                return true;
            }else if(table.isKakumei()){
                if(table.mBafuda.mRankL > yaku.mRankL){
                    return true;
                }
            }else{
                if(table.mBafuda.mRankL < yaku.mRankL){
                    return true;
                }
            }
        }
    }else if(table.isOnset()){
        COUT << "ons" << endl;
        //場が空なら何を出してもいい
        return true;
    }
    return false;
}

void Game::update(const Yaku &yaku){//役での更新
    COUT<<"up"<<endl;
    //ここに来るということはyakuにより更新されることは決まっている。
    //のでyakuの情報で上書きしていけば良い(が少し注意が必要である)
    
    int card[8][15]={{0}};
    yaku.setBitTo815(card);//815に戻す（無駄な作業感）
    players.id[players.turnId()].subtraction(card);//提出カードを差し引く
    players.id[players.turnId()].cards_num -= yaku.mNum;//枚数も差し引く
    if(players.id[players.turnId()].cards_num<0){
        cout << "maisuugaokasii" << endl;
        yaku.print();
        exit(1);
    }
    
    if( players.id[players.turnId()].cards_num == 0 ){//あがった
        players.addAgari( players.turnId() );
    }
    
    if( (config.RULE_8GIRI && yaku.is8giri()) || (config.RULE_SPADE3 && table.mBafuda.isJTanki() && yaku.isSpade3()) ){
        //8切り、スペ3
        table.setOnset(true);//流れる
    }else{
        if( isBaAlive() ){
            table.setOnset(false);
        }else{
            table.setOnset(true);
        }
    }
    
    table.mBafuda = yaku;
    
    //縛りの発生可否
    if( config.RULE_SHIBARI && table.mBafuda.mSuits == yaku.mSuits){
        table.setShibari(1);
    }else{
        table.mBafuda.mSuits = yaku.mSuits;
    }
    //役の種類
    if(yaku.isKaidan()){
        table.setKaidan(true);
    }else if(yaku.isPair()){
        table.setPair(true);
    }else if(yaku.isTanki()){
        table.setTanki(true);
    }
    
    //革命発生可否
    if( config.isKakumei( yaku ) ){
        table.revKakumei();//反転
    }
    
}

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
            yaku->mJposSuit = 0;
            yaku->mJposRank = 14;
            yaku->setJTanki();
            
        }else{
            yaku->mRankR = rank[0];
            yaku->mRankL = rank[0];
            yaku->setSuit(suit[0]);
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
        for(int i=0; i<suit.size(); i++){//jokerの位置を保存
            if(fake[i]){
                yaku->mJposSuit = suit[i];
                yaku->mJposRank = rank[i];
            }
        }
        yaku->setPair();
    }else if(kaidan_flag){//それは階段である
        yaku->set815ToBit( data );
        yaku->mNum = cards_num;
        yaku->mRankL = rank[0];//最初に見つかるものが始点に決まっている
        yaku->mRankR = rank[0]+cards_num-1;
        yaku->setSuit(suit[0]);
        for(int i=0; i<suit.size(); i++){//jokerの位置を保存
            if(fake[i]){
                yaku->mJposSuit = suit[i];
                yaku->mJposRank = rank[i];
            }
        }
        yaku->setKaidan();
    }else{//非受理
        return false;
    }
    
    //この長い審査を通った時ようやくtrueが蛙
    return true;
}

/*
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
    for(int i=0; i<deck.size()*2; i++){
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
    //身分差があれば大富豪を起点、でなければ席順の若い人を起点
    int target = players.convIDtoSekiNum( ( players.isMisalliance() ) ? players.mibunId(DAIFUGO) : players.sekijun[0]);//大富豪から
    
    while( !deck.empty() ){
        players.id[players.sekijun[target]].cards[deck[0]/14][deck[0]%14] = 1;//入れて
        players.id[players.sekijun[target]].cards_num++;//枚数を増やして
        deck.erase( deck.begin() );//削除
        
        //次の人
        target = ( target + 1 ) % ( players.size() );
    }
    
}
*/

void Game::dealCards( Result *result){
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
    for(int i=0; i<deck.size()*2; i++){
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
    //身分差があれば大富豪を起点、でなければ席順の若い人を起点
    int target = players.convIDtoSekiNum( ( players.isMisalliance() ) ? players.mibunId(DAIFUGO) : players.sekijun[0]);//大富豪から
    
    while( !deck.empty() ){
        players.id[players.sekijun[target]].cards[deck[0]/14][deck[0]%14] = 1;//入れて
        players.id[players.sekijun[target]].cards_num++;//枚数を増やして
        deck.erase( deck.begin() );//削除
        
        //次の人
        target = ( target + 1 ) % ( players.size() );
    }
    
    //搾取が行われれば行う
    exploit( result );
}

void Game::dealCards( Result *result, vector<string> &tefudaSet){
    COUT<<"dc"<<endl;
    //カードを配る
    //tefudaSetで
    
    //身分差があれば大富豪を起点、でなければ席順の若い人を起点
    int target = players.convIDtoSekiNum( ( players.isMisalliance() ) ? players.mibunId(DAIFUGO) : players.sekijun[0]);//大富豪から
    string suit = "SHDCJ";
    string rank = "3456789XJQKA2R";
    
    //プレイヤのカードを初期化してから
    for(int i=0; i<players.size(); i++){
        players.id[i].initCard();
    }
    
    for(int i=0; i<config.PLAYER_NUM; i++){
        //一人一人に対して手札セットを割り当てていく
        //cout << tefudaSet[i] << endl;
        for(int j=0; j<tefudaSet[i].length()-1; j++){
            if( tefudaSet[i][j] == ' ' ){
                continue;
            }
            int s=-1, r=-1;
            for(int k=0; k<5; k++){
                //cout << suit[k] << " "<< tefudaSet[i][j] << endl;
                if( suit[k] == tefudaSet[i][j]){
                    s = k;
                    break;
                }
            }
            j++;//1つ進める
            for(int k=0; k<14; k++){
                //cout << rank[k] << " "<< tefudaSet[i][j] << endl;
                if( rank[k] == tefudaSet[i][j]){
                    r = k;
                    break;
                }
            }
            if( s < 0 || r < 0 ){
                //cout << " ~~~~ " << endl;
                exit(1);
            }
            if( s == 4){
                players.id[players.sekijun[target]].cards[ 4 ][ 1 ] = 1;//入れて
            }else{
                //cout << s << " " << r << " " << suit[s] << " " << rank[r] << endl;
                players.id[players.sekijun[target]].cards[ s ][ r+1 ] = 1;//入れて
            }
            players.id[players.sekijun[target]].cards_num++;//枚数を増やして
            
        }
        
        //次の人
        target = ( target + 1 ) % ( players.size() );
    }
    
    /*
    cout << "end dis" << endl;
    
    for(int i=0; i<5; i++){
        cout << i << endl;
        print515( players.id[i].cards );
    }
    */
    //搾取が行われれば行う
    exploit( result );
}

void Game::exploit( Result *result ){
    if( players.isMisalliance() ){//カード交換（搾取）
        //最初の手札通知では、大富豪・富豪には"搾取後"の手札が通知される。
        //一方で大貧民・貧民には"搾取前"の手札が通知される。
        int data[8][15] = {{0}};
        //大貧民から大富豪に
        selectHighCard(data, players.id[players.mibunId(DAIHINMIN)].cards, 2);//2枚選び
        result->addChange(players.mibunId(DAIHINMIN), players.mibunId(DAIFUGO), data);//大貧民の交換カードを保存
        addCard(players.id[players.mibunId(DAIFUGO)].cards, data);//足す
        memset(data, 0, sizeof(data));
        //貧民から富豪に
        selectHighCard(data, players.id[players.mibunId(HINMIN)].cards, 1);//1枚選び
        result->addChange(players.mibunId(HINMIN), players.mibunId(FUGO), data);//貧民の交換カードを保存
        addCard(players.id[players.mibunId(FUGO)].cards, data);//足す
        
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
    /*
    if(table.isChangePhase()){
        temp[5][0] = 1;
    }
    */
    
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
        //temp[6][5+i] = 4-players.id[i].mibun;
        temp[6][5+i] = players.id[i].mibun;
        //席順
        temp[6][10+i] = players.sekijun[i];
    }
    if(!players.isMisalliance()){
        for(int i=0;i<5;i++){
            temp[6][5+i] = 0;
        }
    }
    //現在のターンのプレイヤー番号
    if(players.turn >= 0){
        temp[5][3] = players.turnId();//ターンの人の番号で正しい
        //temp[5][3] = players.turn;
    }
    
    //共通部分をコピー
    for(int i=0;i<5;i++){
        for(int j=5;j<8;j++){
            for(int k=0;k<15;k++){
                data[i][j][k] = temp[j][k];
            }
        }
    }
    
    if(players.turn >= 0){
        data[players.turnId()][5][2] = 1;
    }
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
    
    //場が流れたか
    if(table.isOnset()){
        temp[5][4] = 1;
    }else{
        int pn = 0;
        for(int i=0;i<config.PLAYER_NUM;i++){
            if(players.id[i].passed || players.id[i].cards_num==0 ){//あがってなくてかつパスしていない人がいるなら
                pn++;
            }
        }
        if(pn==config.PLAYER_NUM){
            temp[5][4]=1;
        }
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
        //temp[5][3] = players.turnId();
        temp[5][3] = table.player_id;
        //temp[5][3] = players.turn;
    }
    //cout<<7<<endl;
    
    //共通部分をコピー
    for(int i=0;i<5;i++){
        for(int j=5;j<8;j++){
            for(int k=0;k<15;k++){
                data[i][j][k] = temp[j][k];
            }
        }
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

bool Game::isBaAlive(){
    COUT<<"iba"<<endl;
    //ターンを回せるのか、場が流れないのか
    for(int i=0;i<config.PLAYER_NUM;i++){
        if(players.id[i].passed == false && players.id[i].cards_num!=0 ){//あがってなくてかつパスしていない人がいるなら
            return true;
        }
    }
    return false;
}

void Game::pass(){
    //パスしたことを記録するだけ
    //次のプレイヤーにはここでは回さない
    COUT<<"pass"<<endl;
    players.id[players.turnId()].passed = true;
}

void Game::recover(){
    COUT<<"rec"<<endl;
    for(int i=0;i<players.size();i++){
        players.id[i].passed = false;
    }
}

