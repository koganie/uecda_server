#include<iostream>
#include<string.h>
#include<cstdlib>
#include<unistd.h>
#include"Configure.h"
#include"connection.h"

#include "debug.h"

void acceptClient(const Configure &config, Players *players){
    //�N���C�A���g���󂯕t����
    
    struct sockaddr_in wait_addr; // waiting port
    fd_set target_fds;
	fd_set org_target_fds;
    
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);//�\�P�b�g�����ʂ���L�q�q
    bzero((char *) &wait_addr, sizeof(wait_addr));
    wait_addr.sin_family = PF_INET;
    wait_addr.sin_addr.s_addr = htons(INADDR_ANY);
    wait_addr.sin_port = htons(config.PORT_NUM);

    int t=1;
    int tt=sizeof(t);
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&t, tt);//�\�P�b�g�̃I�v�V�����̐ݒ�
    bind(sockfd,(struct sockaddr *)&wait_addr,sizeof(wait_addr));//sockfd��wait_addr�����蓖�Ă�
    listen(sockfd, 1);//�\�P�b�g(socket)��̐ڑ���҂� 
    
    //player_num�̐������A�����
    for(int i=0; i<config.PLAYER_NUM; i++){
        cout << "now waiting " << i << endl;
        Player player;
        int addrlen = sizeof(player.addr);
        if((player.sockfd = accept(sockfd, (struct sockaddr *)&player.addr, &addrlen)) < 0 ){
            //perror("reader: accept");
            exit(1);
        };
        FD_ZERO(&org_target_fds);
		FD_SET(player.sockfd, &org_target_fds);
		memcpy(&target_fds, &org_target_fds, sizeof(org_target_fds));
        struct timeval waitval;
        waitval.tv_sec  = 2;
		waitval.tv_usec = 500;
        select(50,&target_fds,NULL,NULL,&waitval);
        
        int card[8][15]={{0}};
        recv815(card, player.sockfd);//�v���C���[�̖��O���󂯎��
        player.setName(card);//���O���i�[����
        player.setId(i);//�v���C���[�ԍ����i�[����
        COUT << player.id << " " << player.name << endl;
        send1(i, player.sockfd);//���O��i�Ԗڂ̃v���C���[����
        players->id.push_back(player);//
        players->sekijun.push_back(i);
    } 
}

void gameOver( Players *players ){
    for(int i=0; i<players->id.size(); i++){
        close( players->id[i].sockfd );
    }
}

void send1(int i, int sockfd){
    //�����^�𑗂�
    uint32_t tmp = htonl( (uint32_t)i*(i>=0) );
	write(sockfd, &tmp, sizeof(tmp));
}

void send815(int card[8][15], int sockfd){
	//�J�[�h�𑗂�
    uint32_t tmp[8][15]={{0}};
    
	for(int i=0; i<8; i++){
		for(int j=0; j<15; j++){
		    tmp[i][j] = htonl((uint32_t)card[i][j]*(card[i][j]>=0));
	        //cout << card[i][j] << " ";
		}
	    //cout << endl;
	}
    int i=0;
    write(sockfd, tmp, sizeof(i)*8*15);
	//write(sockfd, tmp, sizeof(tmp));
}

void recv815(int card[8][15], int sockfd){
    uint32_t tmp[8][15]={{0}};	
	
	read(sockfd, tmp, sizeof(tmp));
	for(int i=0; i<8; i++){
		for(int j=0; j<15; j++){
			card[i][j] = (int)ntohl(tmp[i][j]);
		}
	}

}

/*
void sendTefuda( const Configure &config, const Players &players, const Table &table ){
    int data[config.PLAYER_NUM][8][15]={{0}};
    
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
    if(table.isChangePhase()){
        temp[5][0] = 1;
    }
    
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
        temp[6][5+i] = players.id[i].mibun;
        //�ȏ�
        temp[6][10+i] = players.id[i].sekijun;
        
        //���݂̃^�[���̃v���C���[�ԍ�
        temp[5][3] = convSekiToPID(turn);
    }
    
    //���ʕ������R�s�[
    for(int i=0;i<5;i++){
        for(int j=5;j<8;j++){
            for(int k=0;k<15;k++){
                data[i][j][k] = temp[j][k];
            }
        }
    }
    
    //�e�v���C���[�ɂ���ĈႤ����
    for(int i=0;i<5;i++){
        //��������
        if(players[i].mibun == DAIFUGO){
            data[i][5][1] = 2;
        }else if(players[i].mibun == FUGO){
            data[i][5][1] = 1;
        }
        //���̃^�[����
        if(convSekiToPID(turn) == players[i].id){
            data[i][5][2] = 1;
        }
    }
}

void Game::makeTefuda815(int data[5][8][15]){
    
}


*/