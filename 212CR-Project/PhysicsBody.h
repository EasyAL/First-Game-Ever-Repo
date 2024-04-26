#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/freeglut.h>

class PhysicsBody
{
public:

	//	Pure Virtual Functions	//
	virtual void Update(float, int) = 0;
	virtual void AddForce(glm::vec3, glm::vec3) = 0;
	virtual void ResetTotalForces() = 0;

	//	Getters & Setters	//
	virtual float GetMass() = 0;
	virtual glm::vec3 GetPosition() = 0;
	virtual float GetOrientation();
	virtual glm::vec3 GetVelocity() = 0;
	virtual glm::vec3 GetAngularVelocity();

	virtual void SetVelocity(glm::vec3 v) = 0;
	virtual void SetPosition(glm::vec3 p) = 0;
	virtual void SetAngularVelocity(glm::vec3 aV);
	//	Constructors & Destructors	//
	~PhysicsBody();

};

