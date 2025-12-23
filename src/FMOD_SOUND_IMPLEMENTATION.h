#pragma once
// FMOD is a C++ sound playing library (www.fmod.com)
#include <FMOD/fmod.h>
#include <FMOD/fmod_studio.hpp>
#include <FMOD/fmod_errors.h>
#include <thread>


///////////////////////////// Sound /////////////////////////////////////////
void playShooting(FMOD::ChannelGroup* channelGroup, FMOD::Sound* sound, FMOD::System* system) {
	FMOD::Channel* channel;
	system->playSound(sound, channelGroup, false, &channel);
	bullets_shot++;
	Sleep(110);
	ShootingFinished = true;
}
