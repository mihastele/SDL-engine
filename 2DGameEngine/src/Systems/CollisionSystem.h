#pragma once

#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"

class CollisionSystem : public System {
public:
	CollisionSystem() {
		RequireComponent<TransformComponent>(); // for position
		RequireComponent<BoxColliderComponent>();
	}

	void Update() {
		// Check if any of the BoxColliderComponents collide

		auto entities = GetSystemEntities();


		for (auto a = entities.begin(); a != entities.end(); a++) {
			Entity entityA = *a;
			auto aTransform = entityA.GetComponent<TransformComponent>();
			auto aCollider = entityA.GetComponent<BoxColliderComponent>();
			for (auto b = a+1; b != entities.end(); b++) {
				Entity entityB = *b;
				auto bTransform = entityB.GetComponent<TransformComponent>();
				auto bCollider = entityB.GetComponent<BoxColliderComponent>();

				bool collisionHappened = CheckAABBCollision(
					aTransform.position.x + aCollider.offset.x,
					aTransform.position.y + aCollider.offset.y,
					aCollider.width,
					aCollider.height,
					bTransform.position.x + bCollider.offset.x,
					bTransform.position.y + bCollider.offset.y,
					bCollider.width,
					bCollider.height
				);

				if (collisionHappened) {
					// TODO emit events
				}
			}
		}
	}

	bool CheckAABBCollision(double aX, double aY, double aW, double aH, double bX, double bY, double bW, double bH) {
		return (
			aX < bX + bW &&
			aX + aW > bX &&
			aY < bY + bH &&
			aY + aH > bY
			);
	}
};


#endif