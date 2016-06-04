#include<iostream>
#include<vector>
#include<cstdlib>

#include "connection.h"
#include "Configure.h"
#include "Game.h"
//#include "Result.h"

using namespace std;

int main(int argc, char *argv[]){
    
    Configure config(argc, argv);//game�̐ݒ�����߂�
    Players players;
    
    config.printRules();//���[���̏o��
    
    //player_num�l�̃v���C���[��҂�
    acceptClient(config, &players);
    
    Results results( players );//�����̋L�^�p�N���X
    results.clockStart();//�S�����̊J�n���Ԃ�������
    
    //�S�Q�[��
    for(int game_count=1; game_count<=config.GAME_NUM; game_count++){
        
        Result result;//���ʂ�ۑ�����
        
        if(config.isReset(game_count)){//�g���Ɛȏ��̃��Z�b�g
            //cout << "reset" << endl;
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
        
        //result.print();
        
        //�������I��������Ƃ̒ʒm
        for(int i=0; i<config.PLAYER_NUM; i++){
            send1( (game_count < config.GAME_NUM) ? 1 : 2, players.id[i].sockfd );
        }
        
        //���ʂ�ۑ�����
        results.push_back( result );
        results.printOneGameReport();//�ꎎ���̌��ʂ��v�����g����
    }
    
    results.clockEnd();//�S�����̏I�����Ԃ��������Ď������Ԃ��m�F����
    
    //�㏈��
    results.calcScore( config.POINT );//�X�R�A�̌v�Z
    results.calcTransition();//�g���ԑJ�ڂ̌v�Z
    results.print();//�S�o��
    
    gameOver( &players );

    //�t�@�C���ւ̕ۑ�
    cout << " file output start" << endl;
    results.writeScore();
    results.writeTransition();
    cout << " file output done" << endl;

}

