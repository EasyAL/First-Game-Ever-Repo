#pragma once

#include <string>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/freeglut.h>




class Collider
{
protected:
	std::string colliderType;

public:
	Collider();
	Collider(std::string ColliderType);
	~Collider();

	
	std::string GetColliderType();
	
	bool CollideCheck(Collider* otherCollider);

	virtual glm::vec3 GetPosition() = 0;
	virtual bool CollideCheckSphere(Collider* sphere) = 0;
	virtual void SetPosition(glm::vec3 Position) = 0;
	virtual float GetRadius() = 0;

	

};

