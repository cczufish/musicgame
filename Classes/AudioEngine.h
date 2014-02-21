#ifndef __FMOD_H__
#define __FMOD_H__

#include"fmod.hpp"
#include "cocos2d.h"

USING_NS_CC;
using namespace FMOD;

class AudioEngine{
public:
	static AudioEngine* getInstance();
	void create(char* songname);
	void AudioEngine::createLoop(char* songname);
	void play();
	void pause();
	void resume();
	void exit();
	bool isPlaying();
private:
	AudioEngine();
	void init();
	System* system;
	Sound* sound;
	Channel* channel;
	FMOD_RESULT result;

};
#endif 