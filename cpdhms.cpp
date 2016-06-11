#include<iostream>
#include<vector>
#include<cstdlib>
#include<ctime>

#include "connection.h"
#include "Configure.h"
#include "Game.h"
//#include "Result.h"

using namespace std;

#define FILE_OUTPUT //棋譜のファイル出力しない場合はコメントアウト

int main(int argc, char *argv[]){
    
    srand((unsigned int)time(NULL));
    
    Configure config(argc, argv);//gameの設定を収める
    config.printRules();//ルールの出力
    
    Players players;
    
    //player_num人のプレイヤーを待つ
    acceptClient(config, &players);
    
    Results results( players );//試合の記録用クラス
    results.clockStart();//全試合の開始時間をメモる
    
    //全ゲーム
    for(int game_count=1; game_count<=config.GAME_NUM; game_count++){
        
        Result result;//結果を保存する
        
        if(config.isReset(game_count)){//身分と席順のリセット
            players.reset();
            result.reset = true;
            result.sekigae = true;
        }else if(config.isSekigae(game_count)){//席替え（提出の順番）が行われるなれば
            players.sekigae();
            result.reset = false;
            result.sekigae = true;
        }else{
            result.reset = false;
            result.sekigae = false;
        }
        
        //サブゲームを定義する
        Game game(config, players, &result);
        
        //試合を行う
        game.start( &result );//ゲームを開始して結果をresultに収める
        
        //試合が終わった
        result.update( &players );//結果によって更新を行う
        
        //試合が終わったことの通知
        for(int i=0; i<config.PLAYER_NUM; i++){
            send1( (game_count < config.GAME_NUM) ? 1 : 2, players.id[i].sockfd );
        }
        
        //結果を保存する
        results.push_back( result );
        results.printOneGameReport();//一試合の結果をプリントする
        //result.print();//手毎に振り替える
    }
    
    results.clockEnd();//全試合の終了時間をメモって試合時間を確認する
    
    //後処理
    results.calcScore( config.POINT );//スコアの計算
    results.calcTransition();//身分間遷移の計算
    results.print();//全出力
    
    gameOver( &players );

    //ファイルへの保存
    #ifdef FILE_OUTPUT
    cout << " file output start" << endl;
    results.writeScore();
    results.writeTransition();
    cout << " file output done" << endl;
    #endif
}

