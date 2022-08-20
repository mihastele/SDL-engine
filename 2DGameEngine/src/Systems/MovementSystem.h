#pragma once

#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H


#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"

class MovementSystem : public System {
	public:
		MovementSystem() {
			// TODO:
			RequireComponent<TransformComponent>();
			RequireComponent<RigidBodyComponent>();
		}

		void Update(double deltaTime) {
			//TODO:
			// Loop all entities that system is interested in
			
			for (auto entity : GetSystemEntities()) {
				// TODO: Update entity position based on its velocity
				// Every frame of the game loop
				auto& transform = entity.GetComponent<TransformComponent>();
				const auto rigidbody = entity.GetComponent<RigidBodyComponent>();

				transform.position.x += rigidbody.velocity.x * deltaTime;
				transform.position.y += rigidbody.velocity.y * deltaTime;
			}

		}
};

#endif
