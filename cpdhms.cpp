#include<iostream>
#include<vector>
#include<cstdlib>
#include<ctime>

#include "connection.h"
#include "Configure.h"
#include "Game.h"
//#include "Result.h"

using namespace std;

#define FILE_OUTPUT //�����̃t�@�C���o�͂��Ȃ��ꍇ�̓R�����g�A�E�g

int main(int argc, char *argv[]){
    
    srand((unsigned int)time(NULL));
    
    Configure config(argc, argv);//game�̐ݒ�����߂�
    config.printRules();//���[���̏o��
    
    Players players;
    
    //player_num�l�̃v���C���[��҂�
    acceptClient(config, &players);
    
    Results results( players );//�����̋L�^�p�N���X
    results.clockStart();//�S�����̊J�n���Ԃ�������
    
    //�S�Q�[��
    for(int game_count=1; game_count<=config.GAME_NUM; game_count++){
        
        Result result;//���ʂ�ۑ�����
        
        if(config.isReset(game_count)){//�g���Ɛȏ��̃��Z�b�g
            players.reset();
            result.reset = true;
            result.sekigae = true;
        }else if(config.isSekigae(game_count)){//�ȑւ��i��o�̏��ԁj���s����Ȃ��
            players.sekigae();
            result.reset = false;
            result.sekigae = true;
        }else{
            result.reset = false;
            result.sekigae = false;
        }
        
        //�T�u�Q�[�����`����
        Game game(config, players, &result);
        
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
    cout << " file output start" << endl;
    results.writeScore();
    results.writeTransition();
    cout << " file output done" << endl;
    #endif
}

