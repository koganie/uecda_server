#include <iostream>
#include "debug.h"

using namespace std;

void print515(int data[5][15]){
    for(int i=0; i<5; i++){
        for(int j=0; j<15; j++){
            cout << data[i][j] << " ";
        }
        cout << endl;
    }
}

void print815(int data[8][15]){
    for(int i=0; i<8; i++){
        for(int j=0; j<15; j++){
            cout << data[i][j] << " ";
        }
        cout << endl;
    }
}

