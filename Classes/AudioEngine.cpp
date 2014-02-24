#include "AudioEngine.h"

static AudioEngine* engine = nullptr;//����
int beatLastTick = 0;

const int SPEC_LEN = 128;
const float BEAT_THRESHOLD = 0.2f;//�ĵ�������ֵ
const int BEAT_MINLASTTIME = 15;//��С�������֡��
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
	}
	return engine;
}

void AudioEngine::init()
{
	channel = 0;
	result = FMOD::System_Create(&system);
	result = system->init(1, FMOD_INIT_NORMAL, 0);
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

void AudioEngine::play()
{
	result = system->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);
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

bool AudioEngine::isPlaying()
{
	bool x;
	result = channel->isPlaying(&x);
	return x;
}

bool AudioEngine::hasBeat()
{
	double beatavg = 0, beatmax = 0;
	float* specData = new float[SPEC_LEN];

	specData = this->getSpectrum();
	for (int bar = 0; bar < SPEC_LEN; bar++)
	{
		beatavg += specData[bar];
		if (specData[bar]>beatmax)
			beatmax = specData[bar];
	}
	delete[] specData;
	beatavg = beatavg / (float)SPEC_LEN;
	if ((beatmax - beatavg) >= BEAT_THRESHOLD && this->getPosition() - beatLastTick >= BEAT_MINLASTTIME)
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
	x = x * 60 / 1000;//�����֡��
	return x;
}

int AudioEngine::getPosition()
{
	unsigned int x;
	result = channel->getPosition(&x, FMOD_TIMEUNIT_MS);
	x = x * 60 / 1000;//�����֡��
	return x;
}

float* AudioEngine::getSpectrum()
{
	system->update();//ò��Ҫ��ˢ�£�
	float* specData = new float[SPEC_LEN];
	float* specLeft = new float[SPEC_LEN];
	float* specRight = new float[SPEC_LEN];
	result = channel->getSpectrum(specLeft, SPEC_LEN, 0, FMOD_DSP_FFT_WINDOW_RECT);//�õ�������Ƶ��
	result = channel->getSpectrum(specRight, SPEC_LEN, 1, FMOD_DSP_FFT_WINDOW_RECT);//�õ�������Ƶ��
	for (int i = 0; i < SPEC_LEN; i++)
		specData[i] = (specLeft[i] + specRight[i]) / 2;//ƽ����������
	return specData;//����
	delete[] specData;
	delete[] specLeft;
	delete[] specRight;
}

float AudioEngine::getBPM()
{
	float bpmEstimate;//������BPMֵ
	int beatThresholdBar = 0;//�������
	int beatLastTick = 0;//�ϸ�����ʱ��
	int beatIgnoreLastTick = 0;//�����ĵ�
	std::queue<int> beatTimes;//���ĳ�������

	float* specData = new float[SPEC_LEN];
	specData = this->getSpectrum();
	if (specData[beatThresholdBar] >= BEAT_THRESHOLD  && beatLastTick == 0 && beatIgnoreLastTick == 0)
	{
		beatLastTick = GetTickCount();//��⵽���ӣ���¼ʱ��
		beatTimes.push(beatLastTick);//��¼�����ĳ���������
		while (GetTickCount() - beatTimes.front() > beatTrackCutoff)//������ϵ��ĵ�ʱ��̫���˾͵���
		{
			beatTimes.pop();
			if (beatTimes.size() == 0)
				break;
		}
	}

	if (GetTickCount() - beatLastTick < beatSustain)
		int x = 1;//������beat

	else if (beatIgnoreLastTick == 0 && beatLastTick != 0)
	{
		beatLastTick = 0;
		beatIgnoreLastTick = GetTickCount();//�ӳ�ʱ���ڲ��������¸��ĵ�
	}

	if (GetTickCount() - beatIgnoreLastTick >= BEAT_MINLASTTIME)
		beatIgnoreLastTick = 0;//�ӳ�ʱ���ѹ����������¸��ĵ�

	if (beatTimes.size() >= 2)
	{
		float msPerBeat = (beatTimes.back() - beatTimes.front()) / static_cast<float>(beatTimes.size() - 1);//ÿ����beat
		bpmEstimate = 60000 / msPerBeat;
	}
	else
		bpmEstimate = 0;
	return bpmEstimate;
}

