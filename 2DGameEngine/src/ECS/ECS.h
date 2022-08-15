#pragma once

#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>

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
class Component: public IComponent{
	// Returns the unique id of the Component<T>
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

class Registry {

};

template <typename TComponent>
void System::RequireComponent() {
	const auto componentId = Component<TComponent>::GetId();
	componentSignature.set(componentId);
}

#endif