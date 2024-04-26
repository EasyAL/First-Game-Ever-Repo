#include "RigidBody2D.h"

//	Functions	//
void RigidBody2D::Update(float deltaTime, int IntegrationMethod)
{
	//	Rotate the total force by the orientation, hence we move towards where we face
	//totalLinearForce = glm::rotateZ(totalLinearForce, orientation);


	acceleration = totalLinearForce / mass;

	position = position + velocity * deltaTime + 0.5f * acceleration * deltaTime * deltaTime;

	//totalLinearForce = glm::rotateZ(totalLinearForce, orientation);

	velocity = velocity + (acceleration * deltaTime);



	angularAcceleration = totalTorque / momentOfInertia;

	orientation = orientation + angularVelocity.z * deltaTime + 0.5f * angularAcceleration.z * deltaTime * deltaTime;
	angularVelocity = angularVelocity + (angularAcceleration * deltaTime);


	//angularVelocity *= pow(0.05, deltaTime);
	//velocity *= pow(0.25, deltaTime);
}

void RigidBody2D::ResetTotalForces()
{
	totalLinearForce = glm::vec3(0, 0, 0);
	totalTorque = glm::vec3(0, 0, 0);
}

void RigidBody2D::AddForce(glm::vec3 force, glm::vec3 fPos)
{
	totalLinearForce += force;
	//	Simplified cross product equation for 2d vector	//
	totalTorque = glm::cross(fPos, force);
	//totalTorque += fPos.x * force.y - fPos.y * force.x;

}

//	Getters & Setters	//
glm::vec3 RigidBody2D::GetVelocity()
{
	return velocity;
}

glm::vec3 RigidBody2D::GetAngularVelocity()
{
	return angularVelocity;
}

glm::vec3 RigidBody2D::GetPosition()
{
	return position;
}

float RigidBody2D::GetMass()
{
	return mass;
}

float RigidBody2D::GetOrientation()
{
	return orientation;
}


void RigidBody2D::SetPosition(glm::vec3 p)
{
	position = p;
}

void RigidBody2D::SetVelocity(glm::vec3 v)
{
	velocity = v;
}

void RigidBody2D::SetAngularVelocity(glm::vec3 aV)
{
	angularVelocity = aV;
}

//	Constructors & Destructors	//
RigidBody2D::RigidBody2D(glm::vec3 pos, float m, float w, float h)
{
	position = pos;
	mass = m;
	velocity = glm::vec3(0, 0, 0);
	acceleration = glm::vec3(0, 0, 0);
	totalLinearForce = glm::vec3(0, 0, 0);

	orientation = 0;
	angularVelocity = glm::vec3(0, 0, 0);
	angularAcceleration = glm::vec3(0, 0, 0);
	totalTorque = glm::vec3(0, 0, 0);
	momentOfInertia += mass * (w * w + h * h) * 0.083f;	//	Small Optimisation
	//momentOfInertia +=  mass * (w * w + h * h) / 12;
}

RigidBody2D::~RigidBody2D()
{

}










