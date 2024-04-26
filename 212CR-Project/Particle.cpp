#include "Particle.h"
//#include <iostream>

//	Functions	//
void Particle::Update(float deltaTime, int IntegrationMethod)
{
	switch (IntegrationMethod)
	{
	case 1:
		UpdateExplicitEuler(deltaTime);
		break;
	case 2:
		UpdateSemiImplicitEuler(deltaTime);
		break;
	case 3:
		UpdateVelocityVerlet(deltaTime);
		break;
	default:

		break;
	}
}


void Particle::UpdateExplicitEuler(float deltaTime)	//	Explicit Euler
{
	//std::cout << "Using Explicit Euler" << std::endl;

	//	Acceleration = Force / Mass	//
	acceleration = totalLinearForce / mass;

	//	New position = position + (Velocity * deltaTime)	//
	position = position + (velocity * deltaTime);

	//	New Velocity = Velocity + (Acceleration * deltaTime)	//
	velocity = velocity + (acceleration * deltaTime);
}

void Particle::UpdateSemiImplicitEuler(float deltaTime)	//	Semi-Implicit Euler
{
	//std::cout << "Using Semi-Implicit Euler" << std::endl;

	//	Acceleration = Force / Mass	//
	acceleration = totalLinearForce / mass;

	//	New Velocity = Velocity + (Acceleration * deltaTime)	//
	velocity = velocity + (acceleration * deltaTime);

	//	New position = position + (New Velocity * deltaTime)	//
	position = position + (velocity * deltaTime);

	//	Dampening	//
	//velocity *= pow( 0.5, deltaTime);

		//	Make sure to tweak this damping number between 0 and 1 until you are happy with the result.

		//	0 will make it stop instantly when forces are no longer applied
		//	1 will actually increase the amount of velocity added over time (so go a bit below it)
	
}

void Particle::UpdateVelocityVerlet(float deltaTime)	//	Velocity Verlet (2nd Order of magnitude)
{
	position = position + velocity * deltaTime + acceleration * (deltaTime * deltaTime * 0.5f);
	glm::vec3 newAcc = totalLinearForce / mass;
	velocity = velocity + (acceleration + newAcc) * (deltaTime * 0.5f);

	acceleration = totalLinearForce / mass;
}


void Particle::AddForce(glm::vec3 force, glm::vec3 fPos)
{
	totalLinearForce += force;
}

void Particle::ResetTotalForces()
{
	totalLinearForce = glm::vec3(0, 0, 0);
}

//	Getters & Setters	//
float Particle::GetMass()
{
	return mass;
}

glm::vec3 Particle::GetVelocity()
{
	return velocity;
}

glm::vec3 Particle::GetPosition()
{
	return position;
}


void Particle::SetPosition(glm::vec3 p)
{
	position = p;
}


void Particle::SetVelocity(glm::vec3 v)
{
	velocity = v;
}

//	Constructors & Destructors	//
Particle::Particle(glm::vec3 pos, float m)
{
	position = pos;
	mass = m;
	velocity = glm::vec3(0, 0, 0);
	acceleration = glm::vec3(0, 0, 0);
	totalLinearForce = glm::vec3(0, 0, 0);

}

Particle::~Particle()
{

}




