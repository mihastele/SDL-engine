#include "ECS.h"
#include "../Logger/Logger.h"

int IComponent::nextId = 0; // initial value

int Entity::GetId() const {
	return id;
}

void System::AddEntityToSystem(Entity entity) {
	entities.push_back(entity);
}

void System::RemoveEntityFromSystem(Entity entity) {
	entities.erase(std::remove_if(
		entities.begin(),
		entities.end(),
		// lambda/anonymous function
		[&entity](Entity other) {
			return entity == other;
		}
	), entities.end());
}

std::vector<Entity> System::GetSystemEntities() const {
	return entities;
}

const Signature& System::GetComponentSignature() const {
	return componentSignature;
}

Entity Registry::CreateEntity() {
	int entityId;

	entityId = numEntities++;

	Entity entity(entityId);
	entity.registry = this;
	entitiesToBeAdded.insert(entity);

	if (entityId >= entityComponentSignatures.size()) {
		entityComponentSignatures.resize(entityId + 1);
	}

	Logger::Log("Entity created with id = " + std::to_string(entityId));

	return entity;
}

void Registry::AddEntityToSystems(Entity entity) {
	const auto entityId = entity.GetId();

	// TODO Match entityCopmonentSignature <--> systemComponentSignature
	const auto& entityComponentSignature = entityComponentSignatures[entityId];

	// Loop all the systems 
	for (auto& system : systems) {
		const auto& systemComponentsignature = system.second->GetComponentSignature();

		bool isInterested = (entityComponentSignature & systemComponentsignature) == systemComponentsignature; // bitwise comparison

		if (isInterested) {
			// TODO add the entity to the system
			system.second->AddEntityToSystem(entity);
		}
	}

}

void Registry::Update() {
	for (auto entity : entitiesToBeAdded) {
		AddEntityToSystems(entity);
	}
	entitiesToBeAdded.clear();


}
