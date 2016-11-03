#include<iostream>
#include<vector>
#include<cstdlib>
#include<ctime>
#include<random>
#include <functional>

#include "Random.h"
#include "connection.h"
#include "Configure.h"
#include "Game.h"
//#include "Result.h"

using namespace std;

#define FILE_OUTPUT //�����̃t�@�C���o�͂��Ȃ��ꍇ�̓R�����g�A�E�g

int main(int argc, char *argv[]){

    Configure config(argc, argv);//game�̐ݒ�����߂�
    config.printRules();//���[���̏o��
    
    Players players;
    
    //player_num�l�̃v���C���[��҂�
    acceptClient(config, &players);
    
    //�w���D�Z�b�g������Ȃ�ǂݍ���
    vector< vector<string> > tefudaSet;
    if( config.USE_TEFUDA_SET ){
        readTefudaSet( &tefudaSet );
    }

    Random random;//����������
    Results results( players );//�����̋L�^�p�N���X
    results.clockStart();//�S�����̊J�n���Ԃ�������
    
    //�S�Q�[��
    for(int game_count=1; game_count<=config.GAME_NUM; game_count++){
        
        Result result;//���ʂ�ۑ�����
        
        if(config.isReset(game_count)){//�g���̃��Z�b�g
            players.reset();
            result.reset = true;
        }else if(config.isSekigae(game_count)){//�ȑւ��i��o�̏��ԁj���s����Ȃ��
            players.sekigae( &random );
            result.sekigae = true;
        }
        
        //�T�u�Q�[�����`����
        Game game(config, players, &result, &random);
        if( tefudaSet.size() >= game_count){
            //���蓖�Ă�ꂽ��D�Z�b�g������Ȃ炻��Ŏ������s��
            game.dealCards( &result, tefudaSet[game_count-1] );
        }else{
            //�w��̎�D�Z�b�g���Ȃ��̂Ȃ�K���ɕ��z����
            game.dealCards( &result );
        }    
        game.sendFirstCards( &result );//�ŏ��̃J�[�h��ʒm����
        
        //�������s��
        game.start( &result );//�Q�[�����J�n���Č��ʂ�result�Ɏ��߂�
        
        //�������I�����
        result.update( &players );//���ʂɂ���čX�V���s��
        
        //�������I��������Ƃ̒ʒm
        for(int i=0; i<config.PLAYER_NUM; i++){
            send1( (game_count < config.GAME_NUM) ? 1 : 2, players.id[i].sockfd );
        }
        
        //���ʂ�ۑ�����
        results.push_back( result );
        results.printOneGameReport();//�ꎎ���̌��ʂ��v�����g����
        //result.print();//�薈�ɐU��ւ���
    }
    
    results.clockEnd();//�S�����̏I�����Ԃ��������Ď������Ԃ��m�F����
    
    //�㏈��
    results.calcScore( config.POINT );//�X�R�A�̌v�Z
    results.calcTransition();//�g���ԑJ�ڂ̌v�Z
    results.print();//�S�o��
    
    gameOver( &players );

    //�t�@�C���ւ̕ۑ�
    #ifdef FILE_OUTPUT
    if( config.USE_FILE_OUTPUT ){
        cout << " file output start" << endl;
        results.writeScore();
        results.writeTransition();
        //results.writeHistory5();
        cout << " file output done" << endl;
    }else{
        cout << "no output" <<endl;
    }
    #endif
}

