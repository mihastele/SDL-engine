#pragma once

#ifndef ANIMATIONSYSTEM_H
#define ANIMATIONSYSTEM_H

#include <SDL.h>
#include "../ECS/ECS.h"
#include "../Components/AnimationComponent.h"
#include "../Components/SpriteComponent.h"

class AnimationSystem : public System {
public:
	AnimationSystem() {
		RequireComponent<SpriteComponent>();
		RequireComponent<AnimationComponent>();
	}

	void Update() {
		for (auto entity : GetSystemEntities()) {
			auto& animation = entity.GetComponent<AnimationComponent>(); // reference because we're changing a prop
			auto& sprite = entity.GetComponent<SpriteComponent>();

			animation.currentFrame = ((SDL_GetTicks() - animation.startTime)
				* animation.frameSpeedRate / 1000) % animation.numFrames;

			sprite.srcRect.x = animation.currentFrame * sprite.width;
		}
	}
};


#endif
