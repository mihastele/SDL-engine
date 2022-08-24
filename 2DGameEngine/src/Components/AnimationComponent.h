#pragma once


#ifndef ANIMATIONCOMPONENT_H
#define ANIMATIONCOMPONENT_H

#include <SDL.h>

struct AnimationComponent {
	int numFrames;
	int currentFrame;
	int frameSpeedRate;
	bool isLoop;
	int startTime;

	AnimationComponent(int numFrames = 1, int frameSpeedRate = 1, bool shouldLoop = true) {
		this->numFrames = numFrames;
		this->currentFrame = currentFrame;
		this->frameSpeedRate = frameSpeedRate;
		this->isLoop = shouldLoop;
		this->startTime = SDL_GetTicks();
	}

};


#endif