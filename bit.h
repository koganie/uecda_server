typedef unsigned long long int int64;

#define BITull(num)	(1ull<<(num))//unsigned long long int�ɑ΂���
#define CARDBIT(suit, rank)	BITull(suit*13 + (rank-1))//815�z��Ɠ����悤�Ȋ��o��bit���Z���s��

//��r�b�g�ɑ΂��鏈��
#define IS_BA_ONSET     BITull(0)
#define IS_BA_KAIDAN    BITull(1)
#define IS_BA_PAIR      BITull(2)
#define IS_BA_TANKI     BITull(3)
#define IS_BA_KAKUMEI   BITull(4)
#define IS_BA_SHIBARI   BITull(5)
#define IS_BA_JOKER     BITull(6)
#define IS_BA_11BACK    BITull(7)
#define IS_BA_CHANGE_PHASE  BITull(8)

//�J�[�h�r�b�g�ɑ΂��鏈��
//13*3+13-1 = 39+12 = 51
/*
#define IS_SPADE3   BITull(0)
#define IS_JUSED	BITull(52)//joker���g���Ă���
#define IS_KAIDAN   BITull(53)
#define IS_PAIR     BITull(54)
//#define IS_TANKI    BITull(55)
#define IS_PASS     BITull(55)
//#define IS_JTANKI   BITull(57)//joker�P�R�̂��ƁAjoker���g���Ă��邩�ł͂Ȃ�
//#define IS_KAKUMEI  BITull(58)
//#define IS_SHIBARI  BITull(59)
#define IS_8GIRI    BITull(56)
#define IS_NUM(num) BITull(56+num)//�J�[�h�����͉�����?1�ȏオ����
*/
#define IS_SPADE3   BITull(0)
#define IS_JUSED	BITull(52)//joker���g���Ă���
#define IS_KAIDAN   BITull(53)
#define IS_PAIR     BITull(54)
#define IS_TANKI    BITull(55)
#define IS_PASS     BITull(56)
#define IS_JTANKI   BITull(57)//joker�P�R�̂��ƁAjoker���g���Ă��邩�ł͂Ȃ�
#define IS_KAKUMEI  BITull(58)
#define IS_SHIBARI  BITull(59)
//#define IS_8GIRI    BITull(60)
#define IS_8GIRI    CARDBIT(0,6)|CARDBIT(1,6)|CARDBIT(2,6)|CARDBIT(3,6)
//#define IS_NUM(num) BITull(56+num)//�J�[�h�����͉�����?1�ȏオ����

#define DAIFUGO 0
#define FUGO 1
#define HEIMIN 2
#define HINMIN 3
#define DAIHINMIN 4

