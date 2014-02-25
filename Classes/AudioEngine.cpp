#include "AudioEngine.h"

static AudioEngine* engine = nullptr;//单例


const int SPEC_LEN = 127;
const float BEAT_THRESHOLD = 1.2f;//拍点音量阀值
const int BEAT_MINLASTTIME = 15;//最小节奏持续帧数
const int beatSustain = 150;//

const int beatTrackCutoff = 10000;


AudioEngine::AudioEngine()
{

}

AudioEngine* AudioEngine::getInstance()
{
	if (!engine)
	{
		engine = new AudioEngine();
		engine->init();
		engine->initNRT();
	}
	return engine;
}

void AudioEngine::init()
{
	channel = 0;
	result = FMOD::System_Create(&system);
	result = system->init(1, FMOD_INIT_NORMAL, 0);
}

void AudioEngine::initNRT()
{
	channel = 0;
	result = FMOD::System_Create(&systemNRT);
	result = systemNRT->setOutput(FMOD_OUTPUTTYPE_NOSOUND_NRT);
	result = systemNRT->init(1, FMOD_INIT_STREAM_FROM_UPDATE, 0);
}

void AudioEngine::create(const char* songname)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	{
		ssize_t size = 0;
		auto data = FileUtils::getInstance()->getFileData(songname, "r", &size);
		FMOD_CREATESOUNDEXINFO exinfo;
		memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
		exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
		exinfo.length = size;
		result = system->createStream((const char*)data, FMOD_OPENMEMORY | FMOD_LOOP_OFF, &exinfo, &sound);
	}
#else
	result = system->createStream(songname, FMOD_DEFAULT, 0, &sound);
#endif

}

void AudioEngine::createLoop(const char* songname)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	{
		ssize_t size = 0;
		auto data = FileUtils::getInstance()->getFileData(songname, "r", &size);
		FMOD_CREATESOUNDEXINFO exinfo;
		memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
		exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
		exinfo.length = size;
		result = system->createStream((const char*)data, FMOD_OPENMEMORY | FMOD_LOOP_NORMAL, &exinfo, &sound);
	}
#else
	result = system->createStream(songname, FMOD_HARDWARE | FMOD_LOOP_NORMAL | FMOD_2D, 0, &sound);
#endif
}

void AudioEngine::createNRT(const char* songname)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	{
		ssize_t size = 0;
		auto data = FileUtils::getInstance()->getFileData(songname, "r", &size);
		FMOD_CREATESOUNDEXINFO exinfo;
		memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
		exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
		exinfo.length = size;
		result = systemNRT->createStream((const char*)data, FMOD_OPENMEMORY | FMOD_SOFTWARE,, &exinfo, &soundNRT);
	}
#else
	result = systemNRT->createStream(songname, FMOD_SOFTWARE, 0, &sound);
#endif
}

void AudioEngine::play()
{
	result = system->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);
}

void AudioEngine::playNRT()
{
	result = systemNRT->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);
}

void AudioEngine::pause()
{
	result = channel->setPaused(true);
}

void AudioEngine::resume()
{
	result = channel->setPaused(false);
}

void AudioEngine::stop()
{
	result = channel->stop();
}

void AudioEngine::close()
{
	result = sound->release();
	result = system->release();
}

void AudioEngine::update()
{
	systemNRT->update();
}

bool AudioEngine::isPlaying()
{
	bool x;
	result = channel->isPlaying(&x);
	return x;
}

bool AudioEngine::hasBeat()
{
	float DBavg = 0, DBmax = 0;//每一帧的平均dB和最大dB
	static int beatLastTick = 0;
	float* specData = new float[SPEC_LEN];

	specData = this->getSpectrum();
	for (int bar = 0; bar < SPEC_LEN; bar++)
	{
		DBavg += specData[bar];
		if (specData[bar]>DBmax)
			DBmax = specData[bar];
	}
	delete[] specData;
	DBavg = DBavg / SPEC_LEN;
	if ((DBmax / DBavg) >= BEAT_THRESHOLD && this->getPosition() - beatLastTick >= BEAT_MINLASTTIME)
	{
		beatLastTick = this->getPosition();
		return true;
	}
	return false;
}

int AudioEngine::getLength()
{
	unsigned int x;
	result = sound->getLength(&x, FMOD_TIMEUNIT_MS);
	x = x * 60 / 1000;//换算成帧数
	return x;
}

int AudioEngine::getPosition()
{
	unsigned int x;
	result = channel->getPosition(&x, FMOD_TIMEUNIT_MS);
	x = x * 60 / 1000;//换算成帧数
	return x;
}

float* AudioEngine::getSpectrum()
{
	float* specData = new float[SPEC_LEN];
	float* specLeft = new float[SPEC_LEN];
	float* specRight = new float[SPEC_LEN];
	result = channel->getSpectrum(specLeft, SPEC_LEN + 1, 0, FMOD_DSP_FFT_WINDOW_RECT);//得到左声道频谱
	result = channel->getSpectrum(specRight, SPEC_LEN + 1, 1, FMOD_DSP_FFT_WINDOW_RECT);//得到右声道频谱
	for (int i = 0; i < SPEC_LEN; i++)
		specData[i] = (specLeft[i] + specRight[i]) / 2;//平均左右声道
	return specData;//返回
	delete[] specData;
	delete[] specLeft;
	delete[] specRight;
}

float AudioEngine::getBPM()
{
	float bpmEstimate;//待估计BPM值
	int beatThresholdBar = 0;//检测区域
	int beatLastTick = 0;//上个拍子时间
	int beatIgnoreLastTick = 0;//忽略拍点
	std::queue<int> beatTimes;//节拍出现序列

	float* specData = new float[SPEC_LEN];
	specData = this->getSpectrum();
	if (specData[beatThresholdBar] >= BEAT_THRESHOLD  && beatLastTick == 0 && beatIgnoreLastTick == 0)
	{
		beatLastTick = GetTickCount();//检测到拍子，记录时间
		beatTimes.push(beatLastTick);//记录到节拍出现序列中
		while (GetTickCount() - beatTimes.front() > beatTrackCutoff)//如果最老的拍点时间太老了就弹出
		{
			beatTimes.pop();
			if (beatTimes.size() == 0)
				break;
		}
	}

	if (GetTickCount() - beatLastTick < beatSustain)
		int x = 1;//出现了beat

	else if (beatIgnoreLastTick == 0 && beatLastTick != 0)
	{
		beatLastTick = 0;
		beatIgnoreLastTick = GetTickCount();//延迟时间内不允许检测下个拍点
	}

	if (GetTickCount() - beatIgnoreLastTick >= BEAT_MINLASTTIME)
		beatIgnoreLastTick = 0;//延迟时间已过，允许检测下个拍点

	if (beatTimes.size() >= 2)
	{
		float msPerBeat = (beatTimes.back() - beatTimes.front()) / static_cast<float>(beatTimes.size() - 1);//每毫秒beat
		bpmEstimate = 60000 / msPerBeat;
	}
	else
		bpmEstimate = 0;
	return bpmEstimate;
}

