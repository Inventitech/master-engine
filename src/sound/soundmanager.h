/*
 * soundmanager.h
 *
 *  Created on: Oct 17, 2013
 *      Author: nihathrael
 */

#ifndef SOUNDMANAGER_H_
#define SOUNDMANAGER_H_

#include <stdio.h>
#include <stdlib.h>
#include <fmodex/fmod.hpp>
#include <fmodex/fmod_errors.h>

class SoundManager {
	FMOD::System *fmodsystem; //handle to FMOD engine
	FMOD::Channel *channel;
	void init();
	bool checkResult(FMOD_RESULT result);
public:
	SoundManager();
	virtual ~SoundManager();
	FMOD::Sound* createSound(const char* file);
	void deleteSound(FMOD::Sound* sound);
	void playSound(FMOD::Sound* sound);
	bool togglePause();
	bool isPlaying();
	void update();
};

#endif /* SOUNDMANAGER_H_ */
