#ifndef __GLOBAL_H__
#define __GLOBAL_H__

struct Counter//�������ṹ
{
	int frame;//֡��
	int total;//��������
	int perfect;//������
	int good;//��ͨ������
	int miss;//�����
	int combo;//������
	int maxcombo;//���������
};
extern Counter counter;

//��ʼ����Tag
const int INTROSCENE_START = 3;
//������Tag
const int MAINSCENE_OPTION = 7;
const int MAINSCENE_HELP = 8;
const int MAINSCENE_EXIT = 9;
const int MAINSCENE_CAT = 11;
const int MAINSCENE_SHELF = 12;
const int MAINSCENE_TABLE = 13;
const int MAINSCENE_PAPER = 14;
const int MAINSCENE_CLOCK = 15;
const int MAINSCENE_CHECKMARK = 3;
const int MAINSCENE_CROSSMARK = 4;

//��Ϸ����Tag
const int GAMESCENE_PAUSE = 5;
const int GAMESCENE_COMBO = 6;
const int GAMESCENE_JUDGE = 7;
const int GAMESCENE_INFO = 8;
//�������Tag
const int CLEARSCENE_PNO = 6;
const int CLEARSCENE_GNO = 7;
const int CLEARSCENE_MNO = 8;
const int CLEARSCENE_TNO = 11;
const int CLEARSCENE_CNO = 12;
const int CLEARSCENE_COMPLETE = 13;
const int CLEARSCENE_RETRY = 14;
const int CLEARSCENE_RETURN = 15;

#endif 
