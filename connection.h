#ifndef DEFINE_CONNECTION
#define DEFINE_CONNECTION

#include <vector>
#include "Player.h"
#include "Table.h"
#include "Configure.h"

using namespace std;

/*
�N���C�A���g�v���O�����Ƃ̒ʐM�p�̊֐�
*/

//void acceptClient(vector<Player> players, int player_num);//�ΐ킷��N���C�A���g���󂯕t����
//void acceptClient(const Configure &config, vector<Player> &players);
void acceptClient(const Configure &config, Players *players);
void send1(int i, int sockfd);
void send815(int card[8][15], int sockfd);
void recv815(int card[8][15], int sockfd);

void sendTefuda( const Players &players, const Table &table );

void gameOver( Players *players );
#endif
