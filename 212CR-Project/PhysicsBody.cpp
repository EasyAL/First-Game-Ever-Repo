#include "PhysicsBody.h"

//	Functions	//


//	Getters & Setters	//
float PhysicsBody::GetOrientation()
{
	return 1.0f;
}
glm::vec3 PhysicsBody::GetAngularVelocity()
{
	return glm::vec3(0, 0, 0);
}



void PhysicsBody::SetAngularVelocity(glm::vec3 aV)
{

}

//	Constructors & Destructors	//
PhysicsBody::~PhysicsBody()
{

}




