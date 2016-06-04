#ifndef DEFINE_CONFIGURE
#define DEFINE_CONFIGURE

#include<string>

using namespace std;

class Configure{
public:
    int PROTOCOL; //�ʐM�v���g�R���ԍ�
    
    //�Q�[���Ɋւ���ݒ�
    bool RULE_KAKUMEI;//�u�v���v���g����
    bool RULE_SHIBARI;//�u����v���g����
    bool RULE_KAIDAN;//�u�K�i�v���g����
    bool RULE_8GIRI;//�u8�؂�v���g����
    bool RULE_11BACK;//�u11�o�b�N�v���g���� TODO
    bool RULE_MIYAKOOCHI;//�u�s�����v���g���� TODO
    
    int RULE_SEKIGAE_NUM;//�������s���Ɛȏ������Z�b�g���邩
    int RULE_RESET_NUM;//�������s���Ɛg�����̃��Z�b�g���s����
    
    int GAME_NUM;//����̃Q�[�����s����
    int PLAYER_NUM;//�v���C���[�̐�
    
    int POINT;//���_�̉��Z����
    
    int JOKER_NUM;//�W���[�J�[�̖��� TODO
    
    //�ʐM�Ɋւ���ݒ�
    int PORT_NUM;//�|�[�g�ԍ�
    
    //�ݒ�t�@�C��
    char cfg_file[100];
    
    Configure();
    Configure(int argc, char* argv[]);//��������ǂ��邱�񂷂Ƃ炭��
    bool setRule();//cfg_file��ǂݍ���Ń��[�����m�肷��
    
    bool isSekigae(int game_count);//�ȑւ����������邩
    bool isReset(int game_count);//���Z�b�g���������邩
    
    void printRules();
};

#endif

