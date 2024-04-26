#pragma once

#include <glm/glm.hpp>
#include "PhysicsBody.h"

//	vec3 accelerarion, velocity, totalLinearForce, Position	//	
class Particle : public PhysicsBody
{
private:
	//	Variables	//
	float mass;
	glm::vec3 acceleration;
	glm::vec3 velocity;
	glm::vec3 totalLinearForce;

public:
	//	Variables	//
	glm::vec3 position;

	//	Functions	//
	/*
	void UpdateExplicitEuler(float);
	void UpdateSemiImplicitEuler(float);
	void UpdateVelocityVerlet(float);
	*/
	//	Functions	//
	void AddForce(glm::vec3, glm::vec3);
	void Update(float, int);
	void ResetTotalForces();

	void UpdateExplicitEuler(float deltaTime);
	void UpdateSemiImplicitEuler(float deltaTime);
	void UpdateVelocityVerlet(float deltaTime);

	//	Getters & Setters	//
	float GetMass();
	glm::vec3 GetVelocity();
	glm::vec3 GetPosition();

	void SetPosition(glm::vec3 p);
	void SetVelocity(glm::vec3 v);

	//	Constructors & Destructors	//
	Particle(glm::vec3 Position, float Mass);
	~Particle();

};

