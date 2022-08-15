#pragma once

#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

class MovementSystem : public System {
	public:
		MovementSystem() {
			// TODO:
			// RequireComponent<TransformComponent>();
			// RequireComponent<...>();
		}

		void Update() {
			//TODO:
			// Loop all entities that system is interested in
			
			for (auto entity : GetEntities()) {
			// TODO: Update entity position based on its velocity
			// Every frame of the game loop
			}

		}
};

#endif
