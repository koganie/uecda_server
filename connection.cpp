#include<iostream>
#include<string.h>
#include<cstdlib>
#include<unistd.h>
#include"Configure.h"
#include"connection.h"

#include "debug.h"

void acceptClient(const Configure &config, Players *players){
    //クライアントを受け付ける
    
    struct sockaddr_in wait_addr; // waiting port
    fd_set target_fds;
	fd_set org_target_fds;
    
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);//ソケットを識別する記述子
    bzero((char *) &wait_addr, sizeof(wait_addr));
    wait_addr.sin_family = PF_INET;
    wait_addr.sin_addr.s_addr = htons(INADDR_ANY);
    wait_addr.sin_port = htons(config.PORT_NUM);

    int t=1;
    int tt=sizeof(t);
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&t, tt);//ソケットのオプションの設定
    bind(sockfd,(struct sockaddr *)&wait_addr,sizeof(wait_addr));//sockfdにwait_addrを割り当てる
    listen(sockfd, 1);//ソケット(socket)上の接続を待つ 
    
    //player_numの数だけ、入れる
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
        recv815(card, player.sockfd);//プレイヤーの名前を受け取る
        player.setName(card);//名前を格納する
        player.setId(i);//プレイヤー番号を格納する
        COUT << player.id << " " << player.name << endl;
        send1(i, player.sockfd);//お前はi番目のプレイヤーだぞ
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
    //整数型を送る
    uint32_t tmp = htonl( (uint32_t)i*(i>=0) );
	write(sockfd, &tmp, sizeof(tmp));
}

void send815(int card[8][15], int sockfd){
	//カードを送る
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

