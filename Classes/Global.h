#ifndef __GLOBAL_H__
#define __GLOBAL_H__
//计数器结构
struct Counter
{
	int total;//音符总数
	int perfect;//完美数
	int good;//普通击中数
	int miss;//错过数
	int combo;//连击数
	float percent;//总分
};
extern Counter counter;
//开始界面Tag
const int INTROSCENE_START = 3;
const int INTROSCENE_ECHO = 6;
//主界面Tag
const int MAINSCENE_OPTION = 7;
const int MAINSCENE_HELP = 8;
const int MAINSCENE_EXIT = 9;
const int MAINSCENE_CHARACTER = 10;
const int MAINSCENE_CAT = 11;
const int MAINSCENE_SHELF = 12;
const int MAINSCENE_TABLE = 13;
const int MAINSCENE_PAPER = 14;
const int MAINSCENE_CLOCK = 15;
const int MAINSCENE_DIALOGBG = 2;
const int MAINSCENE_CHECKMARK = 3;
const int MAINSCENE_CROSSMARK = 4;
const int MAINSCENE_WORD = 5;
const int MAINSCENE_EXITBG = 6;
//游戏界面Tag
const int GAMESCENE_PAUSE = 5;
const int GAMESCENE_COMBO = 6;
const int GAMESCENE_DIFFICULTY = 7;
const int GAMESCENE_INFO = 19;
const int GAMESCENE_LOADINGBAR = 12;
const int GAMESCENE_PAUSEBG = 3;
const int GAMESCENE_RESUME = 14;
const int GAMESCENE_RETRY = 15;
const int GAMESCENE_OPTION = 16;
const int GAMESCENE_RETURN = 17;
//结算界面Tag
const int CLEARSCENE_BG = 2;
const int CLEARSCENE_TNO = 3;
const int CLEARSCENE_PNO = 4;
const int CLEARSCENE_GNO = 5;
const int CLEARSCENE_MNO = 6;
const int CLEARSCENE_CNO = 7;
const int CLEARSCENE_INFO = 12;
const int CLEARSCENE_JUDGE = 13;
const int CLEARSCENE_RETRY = 14;
const int CLEARSCENE_RETURN = 15;

//其它常数
const int TIME_PRELOAD = 60;//音符提前出现的时间
const std::string FILENAME = "test";//曲子文件名
#endif 
