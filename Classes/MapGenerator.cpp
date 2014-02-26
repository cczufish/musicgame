#include "MapGenerator.h"
#include <fstream>

const float BEAT_THRESHOLD = 1.2f;//拍点音量阀值
const int BEAT_MINLASTTIME = 15;//最小节奏持续帧数
const int FFT_SIZE = 128;

FILE* fout;//输出文件
int beatLastTick;

void MapGenerator::generateMap(const char* songname)
{
	AudioEngine::getInstance()->createNRT(songname);
	AudioEngine::getInstance()->playNRT();
	beatLastTick = 0;
	float* specData = new float[127];
	std::string mapname = songname;
	mapname = FileUtils::getInstance()->getWritablePath() + mapname.substr(mapname.find_last_of('/') + 1, mapname.find_last_of('.') - mapname.find_last_of('/') - 1) + ".gnm";
	fopen_s(&fout, mapname.c_str(), "w");//打开测试谱面
	while (AudioEngine::getInstance()->isPlaying())
	{
		AudioEngine::getInstance()->update();
		if (hasBeat())
		{
			writeNoteline();
		}
	}
	fclose(fout);
	delete[] specData;
	log("%s", "finished");
}

bool MapGenerator::hasBeat()
{
	float DBavg = 0, DBmax = 0;//每一帧的平均dB和最大dB
	float* specData = new float[FFT_SIZE];
	specData = AudioEngine::getInstance()->getSpectrum(FFT_SIZE);
	for (int bar = 0; bar < FFT_SIZE - 1; bar++)
	{
		DBavg += specData[bar];
		if (specData[bar]>DBmax)
			DBmax = specData[bar];
	}
	delete[] specData;
	DBavg = DBavg / FFT_SIZE;
	if ((DBmax / DBavg) >= BEAT_THRESHOLD && AudioEngine::getInstance()->getPosition() - beatLastTick >= BEAT_MINLASTTIME)
	{
		beatLastTick = AudioEngine::getInstance()->getPosition();
		return true;
	}
	return false;
}

void MapGenerator::writeNoteline()
{
	int time = beatLastTick;
	int difficulty = 0;
	int type = 0;
	int length = 0;
	int posX = CCRANDOM_0_1() * 8 + 1;
	int posY = CCRANDOM_0_1() * 8 + 1;
	int desX = CCRANDOM_0_1() * 8 + 1;
	int desY = CCRANDOM_0_1() * 8 + 1;
	fprintf_s(fout, "%.5d,", time);
	fprintf_s(fout, "%.1d,", difficulty);
	fprintf_s(fout, "%.1d,", type);
	fprintf_s(fout, "%.3d,", length);
	fprintf_s(fout, "%.1d%.1d,", posX, posY);
	fprintf_s(fout, "%.1d%.1d\n", desX, desY);
}