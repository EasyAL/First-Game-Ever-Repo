#include "GameObject.h"

std::map<char, bool> GameObject::keys;
std::map<char, bool> GameObject::specialKeys;
int GameObject::integrationMethod = 1;

GameObject::GameObject()
{
	pB = nullptr;
	collider = nullptr;
}

GameObject::~GameObject()
{
	free(pB);
	free(collider);
}

PhysicsBody* GameObject::GetPhysicsBody()
{
	return pB;
}

Collider* GameObject::GetCollider()
{
	return collider;
}

std::string GameObject::GetType()
{
	return "Other";
}

void GameObject::Update(float deltaTime)
{
}