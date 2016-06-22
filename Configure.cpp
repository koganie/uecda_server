#include"Configure.h"
#include <string.h>
#include<iostream>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <ctype.h>

using namespace std;

int isint(char* a){
	/************************************************************/
	/* input  : A string                                        */
	/* return : If a input string are integer and               */
	/*        : the length of the string smaller than 5 then 1, */
	/*        : otherwize 0.                                    */
	/* destroy: none                                            */
	/************************************************************/
	int i;

	if(strlen(a)>=6){
		return 0;
	}
	for(i=0;i<strlen(a);i++){
		if(isdigit(a[i])==0){
			return 0;
		}
	}
	return 1;
}

void strupr2(char *dat){
	while(*dat != '\0'){ 
		//if(*dat >= 'a' && *dat <= 'z')
		// 	*dat = *dat - 0x20;
		*dat=toupper(*dat);
		dat++; 
	}
}

Configure::Configure(){
}
Configure::Configure(int argc, char* argv[]){
    //引数を解読する
    strcpy(cfg_file, "cppdhms.cfg");
    
    for(int i=1; i<=argc-1; i++){
		if(strcmp(argv[i],"-v")==0){ /* print version */
			cout << "tndhms version 0.29" << endl;
		}else if((strcmp(argv[i],"-h")==0)){ /* print help message */
			cout << "tndhms [-vh] [-p port_number]" << endl;
			cout << "   -v version" << endl;
			cout << "   -h help" << endl;
			cout << "   -c config_file" << endl;
			cout << "   -p port_number" << endl;
		}else if((strcmp(argv[i],"-c")==0)){ /* config file name */
		    if(i+1 <= argc-1){
		        FILE *fp;
				fp = fopen(argv[i+1], "r");
				if((strlen(argv[i+1])>=90) || fp==NULL){
					cout << "Bad file name. " << endl;
				}else{
					strcpy(cfg_file, argv[i+1]);
					cout << "a use config_file is " << cfg_file << endl;
				}
				fclose(fp);
			}else{
				cout << "Bad file name. " << endl;
			}
		}else if(strcmp(argv[i],"-p")==0){   /* port number  */
			if(i+1 <= argc-1){
				if(isint(argv[i+1])){
					PORT_NUM = atoi(argv[i+1]);	
					cout << "port number is " << PORT_NUM << endl;
				}else{
					cout << "bad argument" << endl;
				}
			}else{
				cout << "bad argument" << endl;
			}
		}
        /*
        else if(strcmp(argv[i],"-d")==0){
			debug=1;//debug flag
		}
        */
	}
    
    setRule();
}

bool Configure::setRule(){
    FILE *fp;
    fp = fopen(cfg_file,"r");
	if(fp==NULL){
		printf("I can open no config file.\n");
		return 0;
	};	
	int error=0;
	int k=0;
    char *dat1, *dat2, dat3[256], dat4[256];
	while(fgets(dat3, 256, fp) != NULL){
		k++;
		if(dat3[0]=='#'){
			strcpy(dat3," \0");
		}
		strcpy(dat4,dat3);//dat4にdat3をコピー
		dat2=strtok(dat3,"#");//#までをdat2に入れる
		strupr2(dat2);//?
		dat1=strtok(dat2," \r\t\n");//スペースまでdat1に
		if(dat1!=NULL){
		    if(strcmp(dat1,"KAKUMEI_KAIDAN_NUM")==0){
				dat1=strtok(NULL," \r\t\n");
				if(isint(dat1)){
					KAKUMEI_KAIDAN_NUM=atoi(dat1);	
				}else{
					error=1;
				}
			}else if(strcmp(dat1,"KAKUMEI_PAIR_NUM")==0){
				dat1=strtok(NULL," \r\t\n");
				if(isint(dat1)){
					KAKUMEI_PAIR_NUM=atoi(dat1);	
				}else{
					error=1;
				}
			}else if(strcmp(dat1,"RULE_8GIRI")==0){
				dat1=strtok(NULL," \r\t\n");
				if(strcmp(dat1,"YES")==0){
					RULE_8GIRI=1;	
			    }else if(strcmp(dat1,"NO")==0){
					RULE_8GIRI=0;
			    }else{
			        error=1;
			    }
			}else if(strcmp(dat1,"RULE_SPADE3")==0){
				dat1=strtok(NULL," \r\t\n");
				if(strcmp(dat1,"YES")==0){
					RULE_SPADE3=1;	
			    }else if(strcmp(dat1,"NO")==0){
					RULE_SPADE3=0;
			    }else{
			        error=1;
			    }
			}else if(strcmp(dat1,"POINT")==0){
				dat1=strtok(NULL," \r\t\n");
				if(isint(dat1)){
					POINT=atoi(dat1);	
				}else{
					error=1;
				}
			}else if(strcmp(dat1,"PROTOCOL")==0){
				dat1=strtok(NULL," \r\t\n");
				if(isint(dat1)){
					PROTOCOL=atoi(dat1);	
				}else{
					error=1;
				}
			}else if(strcmp(dat1,"GAME_NUM")==0){
				dat1=strtok(NULL," \r\t\n");
				if(isint(dat1)){
					GAME_NUM=atoi(dat1);	
				}else{
					error=1;
				}
			}else if(strcmp(dat1,"RULE_RESET_NUM")==0){
				dat1=strtok(NULL," \r\t\n");
				if(isint(dat1)){
					RULE_RESET_NUM=atoi(dat1);	
				}else{
					error=1;
				}
			}else if(strcmp(dat1,"PORT_NUM")==0){
				dat1=strtok(NULL," \r\t\n");
				if(isint(dat1)){
					PORT_NUM=atoi(dat1);	
				}else{
					error=1;
				}
			}else if(strcmp(dat1,"RULE_KAKUMEI")==0){
				dat1=strtok(NULL," \r\t\n");
				if(strcmp(dat1,"NO")==0){
					RULE_KAKUMEI=0;	
				}else if(strcmp(dat1,"YES")==0){
					RULE_KAKUMEI=1;	
				}else{
					error=1;
				}
			}else if(strcmp(dat1,"RULE_SHIBARI")==0){
				dat1=strtok(NULL," \r\t\n");
				if(strcmp(dat1,"NO")==0){
					RULE_SHIBARI=0;	
				}else if(strcmp(dat1,"YES")==0){
					RULE_SHIBARI=1;	
				}else{
					error=1;
				}
			}else if(strcmp(dat1,"RULE_KAIDAN")==0){
				dat1=strtok(NULL," \r\t\n");
				if(strcmp(dat1,"NO")==0){
					RULE_KAIDAN=0;	
				}else if(strcmp(dat1,"YES")==0){
					RULE_KAIDAN=1;	
				}else{
					error=1;
				}
			}else if(strcmp(dat1,"RULE_8GIRI")==0){
				dat1=strtok(NULL," \r\t\n");
				if(strcmp(dat1,"NO")==0){
					RULE_8GIRI=0;	
				}else if(strcmp(dat1,"YES")==0){
					RULE_8GIRI=1;	
				}else{
					error=1;
				}
			}else if(strcmp(dat1,"RULE_11BACK")==0){
				dat1=strtok(NULL," \r\t\n");
				if(strcmp(dat1,"NO")==0){
					RULE_11BACK=0;	
				}else if(strcmp(dat1,"YES")==0){
					RULE_11BACK=1;	
				}else{
					error=1;
				}
			}else if(strcmp(dat1,"RULE_SEKIGAE_NUM")==0){
				dat1=strtok(NULL," \r\t\n");
				if(isint(dat1)){
					RULE_SEKIGAE_NUM=atoi(dat1);	
				}else{
					error=1;
				}
			}else if(strcmp(dat1,"JOKER_NUM")==0){
				dat1=strtok(NULL," \r\t\n");
				if(isint(dat1)){
				    JOKER_NUM=atoi(dat1);	
				}else{
					error=1;
				}
			}else if(strcmp(dat1,"PLAYER_NUM")==0){
				dat1=strtok(NULL," \r\t\n");
				if(isint(dat1)){
				    PLAYER_NUM=atoi(dat1);	
				}else{
					error=1;
				}
			}

			if(error){
				printf("I found error in line %i\n",k);
				error=0;
			    return false;
			}
		}
	}
	fclose(fp);
    return true;
}

bool Configure::isSekigae(int game_count){
    if(RULE_SEKIGAE_NUM>=1 && game_count%RULE_SEKIGAE_NUM==0){
        return 1;
    }else{
        return 0;
    }
}

bool Configure::isReset(int game_count){
    if(RULE_RESET_NUM>=1 && game_count%RULE_RESET_NUM==0){
        return 1;
    }else{
        return 0;
    }
}

void Configure::printRules(){
    //ルールを出力する
    
    //通信に関する設定
    cout << "PORT:" << PORT_NUM << endl;//ポート番号
    cout << "Configure File Name:" << cfg_file << endl;
    
    cout << "KAKUMEI " << RULE_KAKUMEI << endl; //「革命」を使うか
    cout << "SHIBARI " << RULE_SHIBARI << endl;//「縛り」を使うか
    cout << "KAIDAN " << RULE_KAIDAN << endl;//「階段」を使うか
    cout << "8GIRI " << RULE_8GIRI << endl;//「8切り」を使うか
    cout << "11BACK " <<  RULE_11BACK << endl;//「11バック」を使うか
    
    cout << endl;
    
    cout << "SEKIGAE_NUM " << RULE_SEKIGAE_NUM << endl;//何試合行うと席順をリセットするか
    cout << "RESET_NUM " <<  RULE_RESET_NUM << endl;//何試合行うと身分等のリセットを行うか
    
    cout << "GAME_NUM " << GAME_NUM << endl;//何回のゲームを行うか
    cout << "PLAYER_NUM " <<  PLAYER_NUM << endl;//プレイヤーの数
    
    cout << endl;
}

string Configure::getStr(){
    stringstream str;
    /*
    str += "KAKUMEI";
    str += (RULE_KAKUMEI ? "1\n" : "0\n");
    str += "SHIBARI";
    str += (RULE_SHIBARI ? "1\n" : "0\n");
    str += "KAIDAN";
    str += (RULE_KAIDAN ? "1\n" : "0\n");
    str += "8GIRI";
    str += (RULE_8GIRI ? "1\n" : "0\n");
    str += "11BACK";
    str += (RULE_11BACK ? "1\n" : "0\n");
    str += "SEKIGAE_NUM " << RULE_SEKIGAE_NUM;
    */
    str << "KAKUMEI";
    str << (RULE_KAKUMEI ? "1\n" : "0\n");
    str << "SHIBARI";
    str << (RULE_SHIBARI ? "1\n" : "0\n");
    str << "KAIDAN";
    str << (RULE_KAIDAN ? "1\n" : "0\n");
    str << "8GIRI";
    str << (RULE_8GIRI ? "1\n" : "0\n");
    str << "11BACK";
    str << (RULE_11BACK ? "1\n" : "0\n");
    str << "SEKIGAE_NUM " << RULE_SEKIGAE_NUM;
    
    return str.str();
}

bool Configure::isKakumei( const Yaku &yaku){
    if(yaku.isPair() && yaku.mNum>KAKUMEI_KAIDAN_NUM){
        return true;
    }else if(yaku.isKaidan() && yaku.mNum>KAKUMEI_PAIR_NUM){
        return true;
    }
    return false;
}
