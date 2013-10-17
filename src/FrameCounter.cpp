/*
 * FrameCounter.cpp
 *
 *  Created on: 17 Oct 2013
 *      Author: moritz
 */
#include <stdio.h>
#include <stdlib.h>

#include "FrameCounter.h"

FrameCounter::FrameCounter() :
		fps( { 0.0, 0 }), currentTime(0) {
}

FrameCounter::FrameCounter(double currentTime) :
		fps( { currentTime, 0 }), currentTime(currentTime) {
}

FrameCounter::~FrameCounter() {
}

/**
 * Counts a new frame, and every 5 seconds prints out the
 * average frame rate of the past 5 seconds to the console.
 */
void FrameCounter::countNewFrame(double currentTime) {
	fps.frames++;
	double delta = currentTime - fps.startTime;
	if (delta >= 5) {
		printf("FPS: %f\n", fps.frames / delta);
		fps.frames = 0;
		fps.startTime = currentTime;
	}
}
