#include "Collider.h"

Collider::Collider()
{
    colliderType = "Unknown";
}

Collider::Collider(std::string ColliderType)
{
	colliderType = ColliderType;
}

Collider::~Collider()
{

}



std::string Collider::GetColliderType()
{
	return colliderType;
}


bool Collider::CollideCheck(Collider* otherCollider)
{
    if (otherCollider->GetColliderType() == "Sphere")
        return CollideCheckSphere(otherCollider);

    /*if (otherCollider->GetColliderType() == "AABB")
        CollideCheckAABB(otherCollider);*/
}