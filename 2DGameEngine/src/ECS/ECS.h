#pragma once

#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <set>
#include <memory>
#include <string>
#include "../Logger/Logger.h"

const unsigned int MAX_COMPONENTS = 32;

// We use a bitset of 0s and 1s to help keep track of actve entities
typedef std::bitset<MAX_COMPONENTS> Signature;

// In C++ it is not as common to have one class per file as in Java

struct IComponent {
protected:
	static int nextId;
};

// Used to assign a unique id to componend type
template<typename T>
class Component : public IComponent {
	// Returns the unique id of the Component<T>
public:
	static int GetId() {
		static auto id = nextId++;
		return id;
	}
};


class Entity {
private:
	int id;
public:
	Entity(int id) : id(id) {}; // you can initialize attributes them like so
	Entity(const Entity& entity) = default;
	int GetId() const;

	// Operator overloading is like adding salt. Adding too much of it is not a good idea, use it with care
	Entity& operator = (const Entity& other) = default;
	bool operator == (const Entity& other) const { return id == other.id; }
	bool operator != (const Entity& other) const { return id != other.id; }
	bool operator <= (const Entity& other) const { return id <= other.id; }
	bool operator >= (const Entity& other) const { return id >= other.id; }
	bool operator < (const Entity& other) const { return id < other.id; }
	bool operator > (const Entity& other) const { return id > other.id; }

	template <typename TComponent, typename ...TArgs> void AddComponent(TArgs&& ...args);
	template <typename TComponent> void RemoveComponent();
	template <typename TComponent> bool HasComponent() const;
	template <typename TComponent> TComponent& GetComponent() const;

	// Hold a pointer to the entity's owner registry
	class Registry* registry; // we declare class to let compiler know it exists
};

class System {
private:
	Signature componentSignature;
	std::vector<Entity> entities;

public:
	System() = default;
	~System() = default;

	void AddEntityToSystem(Entity entity);
	void RemoveEntityFromSystem(Entity entity);

	std::vector<Entity> GetSystemEntities() const; // Won't modify entries -> const
	const Signature& GetComponentSignature() const;

	// Defines the component type that entities must have to be considered by the system
	template <typename TComponent> void RequireComponent();
};

// A pool is justa vector (cintuous data) of objects type T

class IPool {
	// We don't want to specify the type class, that is why we made an inherited class (base class)
public:
	virtual ~IPool() {}
};


template <typename T>
class Pool : public IPool {
private:
	std::vector<T> data;

public:
	Pool(int startSize = 100) {
		data.resize(startSize);
	}

	virtual ~Pool() = default;

	bool isEmpty() const {
		return data.empty();
	}

	int GetSize() {
		return data.size();
	}

	void Resize(int n) {
		data.resize(n);
	}

	void Clear() {
		data.clear();
	}

	void Add(T object) {
		data.push_back(object);
	}

	void Set(int index, T object) {
		data[index] = object;
	}

	T& Get(int index) {
		return static_cast<T&>(data[index]);
	}

	T& operator [](unsigned int index) {
		return data[index];
	}

};

class Registry {
	int numEntities = 0;

	// vector of component pools, each pool contains all the data for a certain component type
	// vector index = component type id
	// Pool index = entity id
	// We don't know the type of the Pool, that is why we defined the base Class trick
	std::vector<std::shared_ptr<IPool>> componentPools;

	// Vector of component signatures per entity, saying which component is turned "on for an entity
	// Vector index = entity id
	std::vector<Signature> entityComponentSignatures;

	std::unordered_map<std::type_index, std::shared_ptr<System>> systems;

	std::set<Entity> entitiesToBeAdded;
	std::set<Entity> entitiesToBeKilled;


public:
	Registry() = default;

	void Update();

	Entity CreateEntity();

	void AddEntityToSystem(Entity entity);

	template <typename T, typename ...TArgs> void AddComponent(Entity entity, TArgs&& ...args);

	template <typename TComponent> void RemoveComponent(Entity entity);

	template <typename TComponent> bool HasComponent(Entity entity) const;

	template <typename TComponent> TComponent& GetComponent() const;

	template <typename TSystem, typename ...TArgs> void AddSystem(TArgs&& ...args);

	template <typename TSystem> void RemoveSystem();

	template <typename TSystem> bool HasSystem() const;

	template <typename TSystem> TSystem GetSystem() const;

	// Checks the component signature of an entity and add the entity to the systems
	// that are interested in it
	void AddEntityToSystems(Entity entity);


};

template <typename TComponent>
void System::RequireComponent() {
	const auto componentId = Component<TComponent>::GetId();
	componentSignature.set(componentId);
}



template <typename TComponent, typename ...TArgs> void Registry::AddComponent(Entity entity, TArgs&& ...args) {
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();

	if (componentId >= componentPools.size()) {
		componentPools.resize(componentId + 1, nullptr);
	}

	if (!componentPools[componentId]) {
		std::shared_ptr<Pool<TComponent>> newComponentPool = std::make_shared<Pool<TComponent>>();
		componentPools[componentId] = newComponentPool;
	}

	std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);

	if (entityId >= componentPool->GetSize()) {
		componentPool->Resize(numEntities);
	}

	TComponent newComponent(std::forward<TArgs>(args)...);

	componentPool->Set(entityId, newComponent);

	entityComponentSignatures[entityId].set(componentId);

	Logger::Log("Component id = " + std::to_string(componentId) + "was added to " + std::to_string(entityId));
}

template <typename TComponent> void Registry::RemoveComponent(Entity entity) {
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();

	entityComponentSignatures[entityId].set(componentId, false);

}

// template functions and classes are not classes, they generate classes and functions according to sepcs.
template <typename TComponent> bool Registry::HasComponent(Entity entity) const {
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();

	return entityComponentSignatures[entityId].test(componentId);
}


template<typename TSystem, typename ...TArgs> void Registry::AddSystem(TArgs&& ...args) {
	std::shared_ptr<TSystem> newSystem = std::make_shared<TSystem>(std::forward<TArgs>(args)...); // newSystem(new TSystem(std::forward<TArgs>(args)...));

	systems.insert(std::make_pair(std::type_index(typeid(TSystem)), newSystem));

}

template<typename TSystem> void Registry::RemoveSystem() {
	auto system = systems.find(std::type_index(typeid(TSystem)));
	systems.erase(system);

}

template<typename TSystem> bool Registry::HasSystem() const {
	// find key and if the pointer is different from the end of the map, we found it.
	return systems.find(std::type_index(typeid(TSystem))) != systems.end();

}

template<typename TSystem> TSystem Registry::GetSystem() const {
	auto system = systems.find(std::type_index(typeid(TSystem)));
	return *(std::static_pointer_cast<TSystem>(system->second));
}


#endif