#pragma once

#include <map>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>

#include "Particle.h"
#include "RigidBody2D.h"
#include "SphereCollider.h"
#include "Camera.h"

//#include "SphereCollider.h" //link to your physics (Sphere collider)

class GameObject
{
protected:
	//physics stuff here
	PhysicsBody* pB;	//	pB (Physics Body Instance)
	Collider* collider;


public:
	static int integrationMethod;

	GameObject();
	virtual ~GameObject();

	virtual std::string GetType();

	PhysicsBody* GetPhysicsBody();
	Collider* GetCollider();

	//OpenGL drawing function
	virtual void Draw() = 0;

	///physics update function
	virtual void Update(float);

	//keyboard input
	static std::map<char, bool> keys;
	static std::map<char, bool> specialKeys;
};

