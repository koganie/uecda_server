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
    //GAME�R���X�g���N�^
    //�ŏ��̎�D��z�����āA�ʒm�܂ł���
    COUT<<"GAME"<<endl;
    
    config = c;//�Q�[���̃��[�����i�[����
    players = p;//�v���C���[���R�s�[����
    
    for( int i=0; i<5; i++){
        players.id[i].cards_num = 0;
    }
    
    result->setup( &players );//�v���C���[���ōX�V����
    
    //�ŏ��̎�D�̌���
    dealCards();//�v���C�������ɃJ�[�h�𕪔z����
    
    if( players.isMisalliance() ){//�J�[�h�����i���j
        //�ŏ��̎�D�ʒm�ł́A��x���E�x���ɂ�"����"�̎�D���ʒm�����B
        //����ő�n���E�n���ɂ�"���O"�̎�D���ʒm�����B
        int data[8][15] = {{0}};
        //��n�������x����
        selectHighCard(data, players.id[players.mibunId(DAIHINMIN)].cards, 2);//2���I��
        result->addChange(players.mibunId(DAIHINMIN), players.mibunId(DAIFUGO), data);//��n���̌����J�[�h��ۑ�
        addCard(players.id[players.mibunId(DAIFUGO)].cards, data);//����
        memset(data, 0, sizeof(data));
        //�n������x����
        selectHighCard(data, players.id[players.mibunId(HINMIN)].cards, 1);//1���I��
        result->addChange(players.mibunId(HINMIN), players.mibunId(FUGO), data);//�n���̌����J�[�h��ۑ�
        addCard(players.id[players.mibunId(FUGO)].cards, data);//����
        
    }
    
    players.turn = -1;//�������ɂ̓^�[���̐l���ς��\��������̂ł����ł͓���Ȃ�
    
    //�ŏ��̎�D�𑗕t����
    int tefuda[5][8][15]={{0}};
    makeTefuda815(tefuda);
    for(int i=0;i<config.PLAYER_NUM;i++){
        tefuda[i][5][0] = 1;//�ŏ��Ȃ̂ŃJ�[�h����
        switch(players.id[i].mibun){
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
        
        send815(tefuda[i], players.id[i].sockfd );
    }
    
}

void Game::start( Result *result){
    //�e�v���C���[�̏��ʂ����肷��܂ŃQ�[�����s��
    COUT<<"start"<<endl;
    
    if( players.isMisalliance() ){
        changeCardsUEC( result );//�J�[�h�̌���
    }
    
    players.setCardsNum();
    //�ŏ��̃v���C���[ID��turn�ɕۑ�����
    players.turn = players.convIDtoSekiNum(whoHave(2, 1));//�_�C����3�������̂��ŏ��̃^�[���ł���
    
    while( !isGameEnd() ){//�������������ł���i���ʂ������Ă��Ȃ��j

        {//�e�v���C���̎�D�����A����
            int sendTefuda[config.PLAYER_NUM][8][15]={{0}};
            makeTefuda815( sendTefuda );
            for(int i=0; i<config.PLAYER_NUM; i++){
                send815( sendTefuda[i], players.id[i].sockfd );
            }
        }
        int data[8][15]={{0}};
        recv815(data, players.id[players.turnId()].sockfd);//���݂̃^�[���̃v���C�������o�J�[�h���󂯎��
        
        //��o���ꂽ�f�[�^�̉��
        Yaku yaku;//�󂯎�����f�[�^�̕ϊ���
        int num = 0;//�ԐM�p�ԍ�
        if( conv815toYaku(&yaku, data) ){//815�͂�������"��"�Ƃ��ĉ��߉\���H
            //cout<< "hnkn" <<endl;
            //���߉\�Ȃ��yaku�`���ɕϊ�����Ă���
            if(isSubmittableYaku(yaku)){//��o�\�Ȏ肩�H
                if(yaku.isPass()){//�p�X�Ɩ������Ă���iUEC�W�����[���ɂ͑��݂��Ȃ��j
                    if( config.PROTOCOL!=20070 ){//
                        num = 7;
                    }else{
                        num = 8;
                    }
                }else{//��o�\�Ȗ�
                    num = 9;
                }
            }else{//��o�s�\�Ȗ�
                //�p�X�Ƃ��čX�V����
                yaku.demoPass();
                num = 8;
            }
        }else{//���Ƃ��Đ��藧���Ă��Ȃ�
            //cout << "unable to convert into Yaku!" << endl;
            //print815( data );
            //yaku.printBit();
            yaku.demoPass();
            num = 8;
        }
        
        //���̕ۑ�
        Table bTable = table;//��o�O�̔Ֆ�
        vector<int> passArray, numArray;//�p�X���Ă邩�̔z��
        makePassArray( &passArray );//�p�X�A���C�����
        makeNumArray( &numArray );//�����A���C�����
        result->addAct( players.turn, bTable, passArray, numArray, yaku );//��o���̋L�^
        
        table.player_id = players.turnId();
        
        //�ȉ��A���ۂ̔ՖʍX�V
        switch( num ){
        case 7:
            COUT << 7 << endl;
            send1( 7, players.id[players.turnId()].sockfd );//�p�X�Ƃ��ăp�X������
            break;
        case 8:
            COUT << 8 << endl;
            send1( 8, players.id[players.turnId()].sockfd );//�p�X�����Ƃ݂Ȃ���
            break;
        case 9:
            COUT << 9 << endl;
            //���ɂ��ՖʍX�V
            send1( 9, players.id[players.turnId()].sockfd );//��o�ł���
            update(yaku);//�v���C���[���̍X�V
            break;
        }
        
        {//�X�V���ꂽ��D�𑗂�
            int sendBafuda[config.PLAYER_NUM][8][15] = {{0}};
            makeBafuda815( sendBafuda );//���H
            for(int i=0; i<5; i++){
                send815( sendBafuda[i], players.id[i].sockfd );
            }
        }
            
        //��D��ʒm������A���̃v���C���[�Ɉڂ邪�A�ꂪ����邱�Ƃ�����
        switch(num){
        case 7:
        case 8:
            pass();
            break;
        case 9:
            //�������ʒm�����Ύ��̃v���C���[�Ɉڂ邪�A�ꂪ����邱�Ƃ�����
            if(yaku.is8giri() || (table.isJTanki() && yaku.isSpade3())){
                purge();
                break;
            }
            players.nextTurn();
            break;
        }
        
        {
            //�����I���̔���
            if(isGameEnd()){
                //cout << "GE" << endl;
                //�������I����Ă��܂���
                result->mAgari = players.agari;//�����������Ԃ�result�ɕۑ�����
                for(int i=0; i<players.agari.size(); i++){
                    players.id[players.agari[i]].mibun = i;
                }
                result->finish( players );
                break;//�������
            }else{
                //�܂��I����ĂȂ�
                for(int i=0; i<config.PLAYER_NUM; i++){
                    send1( 0, players.id[i].sockfd );
                }
            }
        }
        
    }//while
    
}

void Game::changeCardsUEC( Result *result ){
    //UECda���[���ɂ���D�̌���
    COUT<<"change"<<endl;
    
    //Game�R���X�g���N�^�ł��łɍ�悳��Ă���
    
    //��������J�[�h���󂯎��A�m�F���s���i�ʐM�j
    //��x�������n����
    int data[8][15] = {{0}};
    recv815(data, players.id[players.mibunId(DAIFUGO)].sockfd);
    if( !checkChangeCardUEC(data, players.id[players.mibunId(DAIFUGO)]) ){//
        cout << " DAIFUGO change faled " << endl;
        //�_���Ȃ�AUEC�T�[�o�͏���ɍ�悵�Ă��܂��悤��
        memset(data, 0, sizeof(data));
        selectHighCard(data, players.id[players.mibunId(DAIFUGO)].cards, 2);
    }
    //PRINT815(data);
    result->addChange(players.mibunId(DAIFUGO), players.mibunId(DAIHINMIN), data);//��x���̌����J�[�h��ۑ�
    deleteCard(players.id[players.mibunId(DAIFUGO)].cards, data);//�����ł���Ȃ�폜����
    addCard(players.id[players.mibunId(DAIHINMIN)].cards, data);//�����t����

    //�x������n����
    memset(data, 0, sizeof(data));
    recv815(data, players.id[players.mibunId(FUGO)].sockfd);
    if( !checkChangeCardUEC(data, players.id[players.mibunId(FUGO)]) ){//
        cout << " FUGO change faled " << endl;
        //�_���Ȃ�AUEC�T�[�o�͏���ɍ�悵�Ă��܂��悤��
        memset(data, 0, sizeof(data));
        selectHighCard(data, players.id[players.mibunId(FUGO)].cards, 1);
    }
    //PRINT815(data);
    result->addChange(players.mibunId(FUGO), players.mibunId(HINMIN), data);//�x���̌����J�[�h��ۑ�
    deleteCard(players.id[players.mibunId(FUGO)].cards, data);//�����ł���Ȃ�폜����
    addCard(players.id[players.mibunId(HINMIN)].cards, data);//�����t����
    
    //�R���X�g���N�^���_�ő�����Ă������̂��C������
    for(int i=0; i<result->mChange.size(); i++){
        if(result->mChange[i].mFrom == players.mibunId(HINMIN)){
            deleteCard(players.id[players.mibunId(HINMIN)].cards, result->mChange[i].mCards);
        }else if(result->mChange[i].mFrom == players.mibunId(DAIHINMIN)){
            deleteCard(players.id[players.mibunId(DAIHINMIN)].cards, result->mChange[i].mCards);
        }
    }
    
}

void Game::make815Table(int card[8][15]){
    COUT<<"m8t"<<endl;
    //card[5][0]=;//�J�[�h�������ł���
    //card[5][1]=;//�J�[�h�����̖���
    //card[5][2]=;//�����̃^�[����
    card[5][3] = players.turn;//���Ԗڂ̃^�[����
    card[5][4] = players.turnId();//�^�[���̃v���C���[�ԍ�
    card[5][5] = table.isOnset();
    card[5][6] = table.is11back();
    card[5][7] = table.isKakumei();
    card[5][8] = table.isShibari();
}

void Game::make815Player(int card[8][15]){
    COUT<<"m8p"<<endl;
    //��D�̖���
    for(int i=0; i<players.size(); i++){
        card[6][0+i] = players.id[i].cards_num;
    }
    //�g��
    for(int i=0; i<players.size(); i++){
        card[6][5+i] = players.id[i].mibun;
    }
    //�ȏ�
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
    if(players.id[0].mibun != HEIMIN || players.id[1].mibun != HEIMIN){//�v���C���[�ɐg���������Ă����
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
    //��o�J�[�h����ɏo����̂�
    
    //�ꂪ����ŏo���X�[�g���قȂ�A�W���[�J�[�P�R�ł��Ȃ�
    if( table.isShibari() && (table.mSuits!=yaku.mSuits) && !yaku.isJTanki() ){
        COUT << "" << endl;
        return false;
    }
    
    if(yaku.isPass()){//���������p�X����C�ł���
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
            if(table.mBafuda.isJTanki() && yaku.isSpade3()){
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

void Game::update(const Yaku &yaku){//���ł̍X�V
    COUT<<"up"<<endl;
    //yaku.print();
    //�����ɗ���Ƃ������Ƃ�yaku�ɂ��X�V����邱�Ƃ͌��܂��Ă���B
    //�̂�yaku�̏��ŏ㏑�����Ă����Ηǂ�
    
    int card[8][15]={{0}};
    //cout << " a" << endl;
    yaku.setBitTo815(card);
    players.id[players.turnId()].subtraction(card);//��o�J�[�h����������
    players.id[players.turnId()].cards_num -= yaku.mNum;//��������������
    if(players.id[players.turnId()].cards_num<0){
        yaku.print();
        exit(1);
    
    }
    if( players.id[players.turnId()].cards_num == 0 ){//��������
        players.addAgari( players.turnId() );
    }
    
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
    if(yaku.isKakumei()){
        table.revKakumei();
    }
    
}

void Game::purge(){//��𗬂�
    COUT<<"pur"<<endl;
    //��D��������������
    table.purge();
    
    //�p�X���畜�A������
    players.recover();
    
    //turn�͍Ō�Ƀp�X�����l�i�܂茻�݂̐l�j�Ȃ̂ŕύX���Ȃ�
    //���A�������Ă���΂��̌���łȂ�
    if( players.id[players.turnId()].cards_num == 0){
        players.nextTurn();
    }
}

bool Game::conv815toYaku(Yaku *yaku, int data[8][15]){//
    COUT<<"c8ty"<<endl;
    //��o�����{���ɒ�o�\���ǂ������m�F���Ayaku�ɕϊ�����
    int cards_num = 0, joker_num = 0;
    vector<int> suit, rank, fake;//�������ꂽ�J�[�h������Ă���
    for(int i=0; i<8; i++){
        for(int j=0; j<15; j++){
            if(data[i][j]==0){//��o����Ă��Ȃ�
                
            }else if(data[i][j]==1){//�ʏ�̃J�[�h����o���ꂽ
                suit.push_back(i);
                rank.push_back(j);
                fake.push_back(0);
                cards_num++;
            }else if(data[i][j]==2){//joker����o���ꂽ
                suit.push_back(i);
                rank.push_back(j);
                fake.push_back(1);
                cards_num++;
                joker_num++;
            }else{//�ςȐ����������Ă���
                return false;
            }
        }
    }
    
    //joker�����̊m�F
    if(joker_num > config.JOKER_NUM){//�����������[���K���葽��
        return false;
    }
    //�ق�Ƃ�joker�������Ă���̂��H
    for(int i=0; i<joker_num; i++){
        if(players.id[players.turnId()].cards[4][1 + i]==0){
            return false;
        }
    }
    //�ق�ƂɃJ�[�h�������Ă���̂��H
    for(int i=0; i<cards_num; i++){
        //joker���g�����J�[�h�ȊO�̂��̂ɂ���
        if(fake[i]==0 && players.id[players.turnId()].cards[suit[i]][rank[i]]==0){
            return false;
        }
    }
    //�Ȃ�΂ǂ̃^�C�v�̉\�������邩�H
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
            //1�ӏ��ł���O�������false�ƂȂ�
            if(suit[i]!=suit[i+1]){//suit���قȂ�
                kaidan_flag=false;
            }
            if(rank[i]!=rank[i+1]){//rank���قȂ�
                pair_flag=false;
            }
        }
    }
    
    if(tanki_flag){//����͒P�R�ł���
        yaku->set815ToBit( data );
        yaku->mNum = cards_num;
        if(joker_num == 1){//joker�P�R
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
    }else if(pair_flag){//����̓y�A�ł���
        yaku->set815ToBit( data );
        yaku->mNum = cards_num;
        yaku->mRankR = rank[0];
        yaku->mRankL = rank[0];
        for(int i=0; i<suit.size(); i++){
            yaku->setSuit(suit[i]);
        }
        for(int i=0; i<suit.size(); i++){//joker�̈ʒu��ۑ�
            if(fake[i]){
                yaku->mJposSuit = suit[i];
                yaku->mJposRank = rank[i];
            }
        }
        yaku->setPair();
    }else if(kaidan_flag){//����͊K�i�ł���
        yaku->set815ToBit( data );
        yaku->mNum = cards_num;
        yaku->mRankL = rank[0];//�ŏ��Ɍ�������̂��n�_�Ɍ��܂��Ă���
        yaku->mRankR = rank[0]+cards_num-1;
        yaku->setSuit(suit[0]);
        for(int i=0; i<suit.size(); i++){//joker�̈ʒu��ۑ�
            if(fake[i]){
                yaku->mJposSuit = suit[i];
                yaku->mJposRank = rank[i];
            }
        }
        yaku->setKaidan();
    }else{//���
        return false;
    }
    
    //���̒����R����ʂ������悤�₭true���^
    return true;
}

void Game::dealCards(){
    COUT<<"dc"<<endl;
    //�J�[�h��z��
    
    vector<int> deck;
    
    //�ʏ�̃J�[�h
    for(int i=0; i<4; i++){
        for(int j=1; j<=13; j++){
            deck.push_back( i*14 + j );
        }
    }
    //joker�̖���
    for(int i=0; i<config.JOKER_NUM; i++){
        deck.push_back( 14*4 + 1 );
    }
    
    //deck��shuffle
    for(int i=0; i<deck.size()*2; i++){
        int a = (int)( (deck.size()-1) * ((double)random()/RAND_MAX));
        int b = (int)( (deck.size()-1) * ((double)random()/RAND_MAX));
        int c = deck[a];
        deck[a] = deck[b];
        deck[b] = c;
    }
    
    //�v���C���̃J�[�h�����������Ă���
    for(int i=0; i<players.size(); i++){
        players.id[i].initCard();
    }
    //deck���ꖇ�����z���Ă���
    //�g����������Α�x�����N�_�A�łȂ���ΐȏ��̎Ⴂ�l���N�_
    int target = players.convIDtoSekiNum( ( players.isMisalliance() ) ? players.mibunId(DAIFUGO) : players.sekijun[0]);//��x������
    
    while( !deck.empty() ){
        players.id[players.sekijun[target]].cards[deck[0]/14][deck[0]%14] = 1;//�����
        players.id[players.sekijun[target]].cards_num++;//�����𑝂₵��
        deck.erase( deck.begin() );//�폜
        
        //���̐l
        target = ( target + 1 ) % ( players.size() );
    }
}

bool checkChangeCardUEC(int data[8][15], Player &player){
    COUT<<"ccc"<<endl;
    //�����\�ȃJ�[�h�Ȃ̂��H
    int cnt = 0;
    bool gacchi = true;
    
    for(int i=0; i<5; i++){
        for(int j=0; j<15; j++){
            if(data[i][j]!=0){
                cnt++;
                if( player.cards[i][j]==0 ){//�����Ă��Ȃ�������
                    gacchi = false;
                }
            }
        }
    }
    if( player.mibun == FUGO ){//�x���Ȃ�1��
        if( cnt == 1 && gacchi ){
            return true;
        }else{
            return false;
        }
    }else if( player.mibun == DAIFUGO ){//��x���Ȃ�2��
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
    //UECda�`���́A���M����815�`���̔z����쐬����
    for(int i=0; i<5; i++){//�v���C���[
        for(int j=0; j<8; j++){//�X�[�g
            for(int k=0; k<15; k++){//�����N
                data[i][j][k] = 0;
            }
        }
    }
    //�J�[�h��
    for(int i=0; i<5; i++){//�v���C���[
        for(int j=0; j<4; j++){//�X�[�g
            for(int k=1; k<=13; k++){//�����N
                if(players.id[i].cards[j][k]!=0){
                    data[i][j][k] = 1;
                }
            }
        }        
        //�W���[�J�[
        for(int j=0; j<config.JOKER_NUM; j++){
            if(players.id[i].cards[4][1+j]!=0){
                data[i][4][1+j] = 2;
            }
        }
    }
    
    //�Ֆʕ�
    int temp[8][15]={{0}};
    //�����t�F�[�Y��
    /*
    if(table.isChangePhase()){
        temp[5][0] = 1;
    }
    */
    
    //�ꂪ���ꂽ��
    if(table.isOnset()){
        temp[5][4] = 1;
    }
    //�C���u���o�b�N
    if(table.is11back()){
        temp[5][5] = 1;
    }
    //�v��
    if(table.isKakumei()){
        temp[5][6] = 1;
    }
    //���΂�
    if(table.isShibari()){
        temp[5][7] = 1;
    }
    
    //�v���C���[����
    for(int i=0;i<5;i++){
        //��D�̖���
        temp[6][0+i] = players.id[i].cards_num;
        //�K��
        //temp[6][5+i] = 4-players.id[i].mibun;
        temp[6][5+i] = players.id[i].mibun;
        //�ȏ�
        temp[6][10+i] = players.sekijun[i];
    }
    if(!players.isMisalliance()){
        for(int i=0;i<5;i++){
            temp[6][5+i] = 0;
        }
    }
    //���݂̃^�[���̃v���C���[�ԍ�
    if(players.turn >= 0){
        temp[5][3] = players.turnId();//�^�[���̐l�̔ԍ��Ő�����
        //temp[5][3] = players.turn;
    }
    
    //���ʕ������R�s�[
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
    
    //UECda�`���́A���M����815�`���̔z����쐬����
    for(int i=0; i<5; i++){//�v���C���[
        for(int j=0; j<8; j++){//�X�[�g
            for(int k=0; k<15; k++){//�����N
                data[i][j][k] = 0;
            }
        }
    }
    //�J�[�h��
    int bafuda[8][15] = {{0}};
    table.mBafuda.setBitTo815(bafuda);
    for(int i=0; i<5; i++){//�v���C���[
        for(int j=0; j<5; j++){//�X�[�g
            for(int k=0; k<15; k++){
                data[i][j][k] = bafuda[j][k];
            }
        }
    }
    //�Ֆʕ�
    int temp[8][15]={{0}};
    //�����t�F�[�Y��
    /*
    if(table.isChangePhase()){
        temp[5][0] = 1;
    }
    */
    //cout<<2<<endl;
    //�ꂪ���ꂽ��
    if(table.isOnset()){
        temp[5][4] = 1;
    }
    //cout<<3<<endl;
    //�C���u���o�b�N
    if(table.is11back()){
        temp[5][5] = 1;
    }
    //cout<<4<<endl;
    //�v��
    if(table.isKakumei()){
        temp[5][6] = 1;
    }
    //cout<<5<<endl;
    //���΂�
    if(table.isShibari()){
        temp[5][7] = 1;
    }
    //cout<<6<<endl;
    
    //�v���C���[����
    for(int i=0;i<5;i++){
        //��D�̖���
        temp[6][0+i] = players.id[i].cards_num;
        //�K��
        temp[6][5+i] = players.id[i].mibun;
        //�ȏ�
        temp[6][10+i] = players.sekijun[i];
        
        //���݂̃^�[���̃v���C���[�ԍ�
        //temp[5][3] = convSekiToPID(turn);
        //temp[5][3] = players.turnId();
        temp[5][3] = table.player_id;
        //temp[5][3] = players.turn;
    }
    //cout<<7<<endl;
    
    //���ʕ������R�s�[
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
    //�Q�[���͏I��������H
    int agari_num = 0;
    int agattenai = 0;
    for(int i=0; i<config.PLAYER_NUM; i++){
        if(players.id[i].cards_num == 0){
            agari_num++;
        }else{
            agattenai = i;//�Ō�̏オ���ĂȂ��l
        }
    }
    
    if(agari_num==config.PLAYER_NUM || agari_num==(config.PLAYER_NUM-1)){
        players.addAgari( agattenai );
        return true;
    }else{
        return false;
    }
}

void Game::pass(){
    COUT<<"pass"<<endl;
    players.id[players.turnId()].passed = true;
    for(int i=0;i<config.PLAYER_NUM;i++){
        if(players.id[i].passed == false && players.id[i].cards_num!=0 ){//�������ĂȂ��Ă��p�X���Ă��Ȃ��l������Ȃ�
            players.nextTurn();//���̐l
            return;
        }
    }
    //�S���p�X���Ă���
    purge();
}

void Game::recover(){
    COUT<<"rec"<<endl;
    for(int i=0;i<players.size();i++){
        players.id[i].passed = false;
    }
}
