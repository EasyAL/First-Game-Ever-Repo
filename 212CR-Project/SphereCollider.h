#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Collider.h"

class SphereCollider	:	public Collider
{
private:
	//	Variables	//
	glm::vec3 centerPos;
	float radius;

public:
	//	Functions	//
	//bool CollideCheck(Collider* otherCollider);
	bool CollideCheckSphere(Collider* sphere);
	//	Setters & Getters	//
	void SetPosition(glm::vec3 position);
	glm::vec3 GetPosition();
	float GetRadius();

	//	Constructors & Destructors	//
	SphereCollider();
	SphereCollider(glm::vec3 Position, float Radius);
	~SphereCollider();

};

