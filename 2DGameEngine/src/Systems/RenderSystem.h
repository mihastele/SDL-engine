#pragma once


#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "../AssetStore/AssetStore.h"
#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include <SDL.h>
#include <algorithm>

class RenderSystem : public System {
public:

	RenderSystem() {
		RequireComponent<TransformComponent>();
		RequireComponent<SpriteComponent>();
	}

	void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore) {
		// TODO sort entities by the z-index (this is unoptimal in update for large amount of entities)


		struct RenderableEntity {
			TransformComponent transformComponent;
			SpriteComponent spriteComponent;
		};
		std::vector<RenderableEntity> renderableEntities;
		for (auto entity : GetSystemEntities()) {
			RenderableEntity renderableEntity;
			renderableEntity.spriteComponent = entity.GetComponent<SpriteComponent>();
			renderableEntity.transformComponent = entity.GetComponent<TransformComponent>();

			renderableEntities.emplace_back(renderableEntity);
		}

		// sort the vector by the z-index value
		std::sort(renderableEntities.begin(), renderableEntities.end(), [](const RenderableEntity& a, const RenderableEntity& b) {
			return a.spriteComponent.zIndex < b.spriteComponent.zIndex;
			});


		for (auto entity : GetSystemEntities()) {
			const auto transform = entity.GetComponent<TransformComponent>();
			const auto sprite = entity.GetComponent<SpriteComponent>();

			
			// set the rects of the original sprite texture

			//srcRect is useful fo tilemaps
			SDL_Rect srcRect = sprite.srcRect;
			SDL_Rect destRect = {
				static_cast<int>(transform.position.x),
				static_cast<int>(transform.position.y),
				static_cast<int>(sprite.width * transform.scale.x),
				static_cast<int>(sprite.height * transform.scale.y)
			};

			SDL_RenderCopyEx(
				renderer,
				assetStore->GetTexture(sprite.assetId),
				&srcRect, &destRect,
				transform.rotation,
				NULL,
				SDL_FLIP_NONE);

		}
	}
};

#endif