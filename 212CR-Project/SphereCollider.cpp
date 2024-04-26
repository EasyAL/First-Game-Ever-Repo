#include "SphereCollider.h"
//#include<iostream>

//	Functions	//

bool SphereCollider::CollideCheckSphere(Collider* sphere)
{
    float distance = sqrt(
        (centerPos.x - sphere->GetPosition().x) * (centerPos.x - sphere->GetPosition().x) +
        (centerPos.y - sphere->GetPosition().y) * (centerPos.y - sphere->GetPosition().y) +
        (centerPos.z - sphere->GetPosition().z) * (centerPos.z - sphere->GetPosition().z));
    // std::cout << radius << "    " << otherSphere->radius << std::endl;
    float totalRadius = radius + sphere->GetRadius();
    //std::cout << totalRadius << std::endl;
    //std::cout << distance << std::endl;
   // std::cout << "(" << sphere->GetPosition().x << ", " << sphere->GetPosition().y << ", " << sphere->GetPosition().z << ")" << std::endl;
    if (totalRadius >= distance)
    {
        return true;
    }
    return false;         //distance <= (radius + otherSphere->radius);
}


//	Getters & Setters	//

glm::vec3 SphereCollider::GetPosition()
{
    return centerPos;
}

float SphereCollider::GetRadius()
{
    return radius;
}



void SphereCollider::SetPosition(glm::vec3 Position)
{
    centerPos = Position;
}

//	Constructors & Destructors	//
SphereCollider::SphereCollider()    :   Collider("Sphere")
{
    centerPos = glm::vec3(0.f, 0.f, 0.f);
    radius = 1.0f;
}

SphereCollider::SphereCollider(glm::vec3 Position, float Radius)  : Collider("Sphere")
{
    centerPos = Position;
    radius = Radius;
}

SphereCollider::~SphereCollider()
{

}




