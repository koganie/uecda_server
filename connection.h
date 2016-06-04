#ifndef DEFINE_CONNECTION
#define DEFINE_CONNECTION

#include <vector>
#include "Player.h"
#include "Table.h"
#include "Configure.h"

using namespace std;

/*
クライアントプログラムとの通信用の関数
*/

//void acceptClient(vector<Player> players, int player_num);//対戦するクライアントを受け付ける
//void acceptClient(const Configure &config, vector<Player> &players);
void acceptClient(const Configure &config, Players *players);
void send1(int i, int sockfd);
void send815(int card[8][15], int sockfd);
void recv815(int card[8][15], int sockfd);

void sendTefuda( const Players &players, const Table &table );

void gameOver( Players *players );
#endif
