#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp >
#include "PhysicsBody.h"

class RigidBody2D : public PhysicsBody
{
private:
	//	Variables	//
	//	Linear Motion	//
	float mass;
	glm::vec3 acceleration;
	glm::vec3 velocity;
	glm::vec3 totalLinearForce;

	//	Rotational Motion	//
	glm::vec3 angularAcceleration;
	glm::vec3 angularVelocity;
	glm::vec3 totalTorque;	//	totalAngularForce
	glm::vec3 momentOfInertia;

public:
	//	Variables	//
	glm::vec3 position;
	float orientation;

	//	Functions	//
	void AddForce(glm::vec3, glm::vec3);
	void Update(float, int);
	void ResetTotalForces();

	//	Getters & Setters	//
	float GetOrientation();
	glm::vec3 GetPosition();
	float GetMass();
	glm::vec3 GetVelocity();
	glm::vec3 GetAngularVelocity();

	void SetPosition(glm::vec3 p);
	void SetVelocity(glm::vec3 Velocity);
	void SetAngularVelocity(glm::vec3 AngularVelocity);

	//	Constructors & Destructors	//
	RigidBody2D(glm::vec3 Position, float Mass, float Width, float Height);
	~RigidBody2D();

};

