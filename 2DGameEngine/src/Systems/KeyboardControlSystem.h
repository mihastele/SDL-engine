#pragma once

#ifndef KEYBOARDCONTROLSYSTEM_H
#define KEYBOARDCONTROLSYSTEM_H

#include "../Components/KeyboardControlledComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"

class KeyboardControlSystem : public System {
public:
	KeyboardControlSystem() {
		RequireComponent<KeyboardControlledComponent>();
		RequireComponent<RigidBodyComponent>();
		RequireComponent<SpriteComponent>();
	}

	void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
		eventBus->SubscribeToEvent<KeyPressedEvent>(this, &KeyboardControlSystem::OnKeyPressed);
	}

	void OnKeyPressed(KeyPressedEvent& event) {
		auto keyCode = event.symbol;
		// std::string keySymbol(1, event.symbol);

		for (auto entity : GetSystemEntities()) {
			const auto keyboardControl = entity.GetComponent<KeyboardControlledComponent>(); // not going to change
			auto& sprite = entity.GetComponent<SpriteComponent>();
			auto& rigidBody = entity.GetComponent<RigidBodyComponent>();

			switch (keyCode) {
				case SDLK_UP:
					rigidBody.velocity = keyboardControl.upVelocity;
					sprite.srcRect.y = sprite.height * 0;
					break;
				case SDLK_RIGHT:
					rigidBody.velocity = keyboardControl.rightVelocity;
					sprite.srcRect.y = sprite.height * 1;
					break;
				case SDLK_DOWN:
					rigidBody.velocity = keyboardControl.downVelocity;
					sprite.srcRect.y = sprite.height * 2;
					break;
				case SDLK_LEFT:
					rigidBody.velocity = keyboardControl.leftVelocity;
					sprite.srcRect.y = sprite.height * 3;
					break;
			}
		}
	}

	void Update() {

	}
};

#endif
