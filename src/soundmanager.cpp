/*
 * soundmanager.cpp
 *
 *  Created on: Oct 17, 2013
 *      Author: nihathrael
 */

#include "soundmanager.h"

SoundManager::SoundManager() {
	init();
}

SoundManager::~SoundManager() {
	fmodsystem->close();
	fmodsystem->release();
}

void SoundManager::init() {
	FMOD_RESULT result = FMOD::System_Create(&fmodsystem); // create an instance of the game engine
	checkResult(result);
	uint version;
	result = fmodsystem->getVersion(&version);
	checkResult(result);

	result = fmodsystem->init(1, FMOD_INIT_NORMAL, 0);
	checkResult(result);
}

bool SoundManager::checkResult(FMOD_RESULT result) {
	if (result != FMOD_OK) {
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		return false;
	}
	return true;
}

/**
 * Load a file into a sound.
 * @return The newly created sound. It has to be deleted using deleteSound() after usage!
 */
FMOD::Sound* SoundManager::createSound(const char* file) {
	FMOD::Sound* sound;
	FMOD_RESULT result = fmodsystem->createSound(file,
		FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &sound);
	checkResult(result);
	sound->setMode(FMOD_LOOP_OFF);
	return sound;
}

/**
 * Delete a sound and free used memory
 */
void SoundManager::deleteSound(FMOD::Sound* sound) {
	sound->release();
}

/**
 * Start playing the given sound
 */
void SoundManager::playSound(FMOD::Sound* sound) {
	fmodsystem->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);
}

/**
 * Toggle pause on active channel
 */
bool SoundManager::togglePause() {
	bool paused;
	channel->getPaused(&paused);
	channel->setPaused(!paused);
	return !paused;
}

/**
 * Returns whether the active channel is playing a sound.
 */
bool SoundManager::isPlaying() {
	bool isPlaying;
	channel->isPlaying(&isPlaying);
	return isPlaying;
}

/**
 * Needs to be called every frame to make sure the sound keeps playing.
 */
void SoundManager::update() {
	fmodsystem->update();
}
