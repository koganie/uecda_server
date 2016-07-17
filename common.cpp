#include "common.h"

void addCard(int c1[8][15], int c2[8][15]){
    //c1��c2��������
    for(int i=0; i<4; i++){
        for(int j=0; j<15; j++){
            if(c2[i][j] != 0){
                c1[i][j] = 1;
            }
        }
    }
    //joker
    for(int i=0; i<15; i++){
        if(c2[4][i] != 0){
            c1[4][i] = 2;
        }
    }
}

void deleteCard(int c1[8][15], int c2[8][15]){
    //c1����c2������
    for(int i=0; i<4; i++){
        for(int j=0; j<15; j++){
            if(c2[i][j] != 0){
                c1[i][j] = 0;
            }
        }
    }
    //joker
    for(int i=0; i<15; i++){
        if(c2[4][i] != 0){
            c1[4][i] = 0;
        }
    }
}

void deleteCard2(int c1[8][15], int c2[8][15]){
    //c1����c2������
    for(int i=0; i<5; i++){
        for(int j=0; j<15; j++){
            if(c2[i][j] == 1){
                c1[i][j] = 0;
            }
            else if(c2[i][j] == 2){
                c1[4][1] = 0;
            }
        }
    }
}

void copyCard(int c1[8][15], int c2[8][15]){
    //c1����c2������
    for(int i=0; i<5; i++){
        for(int j=0; j<15; j++){
            c1[i][j] = c2[i][j];
        }
    }
}

void selectHighCard(int dest[8][15], int src[8][15], int num){
    //src�̃����N�̍���������n����dest�ɓ����
    int n = 0;
    //joker����
    for(int i=0; i<=14 && n<num; i++){
        if(src[4][14-i]!=0){
            dest[4][14-i]=2;
            n++;
        }
    }
    //���ʂ̃J�[�h��
    for(int i=14; i>=0 && n<num; i--){
        for(int j=0; j<4 && n<num; j++){
            if(src[j][i]!=0){
                dest[j][i] = 1;
                n++;
            }
        }
    }
    
}

int countCard(int card[8][15]){
    //card�𐔂��グ��
    int cnt = 0;
    for(int i=0; i<5; i++){
        for(int j=0; j<15; j++){
            if( card[i][j]!=0 ){
                cnt++;
            }
        }
    }
    return cnt;
}
