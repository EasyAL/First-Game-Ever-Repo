#include "ForceGen.h"

//	Functions	//
void ForceGen::UpdateForces()
{
	ApplyGravity();
}


void ForceGen::ApplyGravity()
{
	for (int i = 0; i < gravityGen.size(); i++)
	{
		//	Multiplying the particles mass by the gravity force	and adding it onto the total linear force for object i	//
		//	This is a simplified formula for gravity, f = g ((m1 * m2) / (r * r)); we have f = g * m1 as m2 and r are both constant	//
		gravityGen[i]->GetPhysicsBody()->AddForce(gForce * gravityGen[i]->GetPhysicsBody()->GetMass(), glm::vec3(0, 1, 0));     // ->GetParticle()->AddForce(gForce * gravityGen[i]->GetParticle()->getMass());
	}
}

void ForceGen::AddGravity(GameObject* object)
{
	gravityGen.push_back(object);
}

void ForceGen::RemoveGravity(GameObject* object)
{
	for (int i = 0; i < gravityGen.size(); i++)
	{
		if (gravityGen[i] == object)
		{
			gravityGen.erase(gravityGen.begin() + i);
			gravityGen.shrink_to_fit();
			break;
		}
	}
}


void ForceGen::ApplyDrag(float deltaTime)
{
	glm::vec3 v;
	glm::vec3 aV;
	for (int i = 0; i < dragGen.size(); i++)
	{
		v = dragGen[i]->GetPhysicsBody()->GetVelocity();
		aV = dragGen[i]->GetPhysicsBody()->GetAngularVelocity();

		dragGen[i]->GetPhysicsBody()->SetVelocity(v *= pow(0.5f, deltaTime));
		dragGen[i]->GetPhysicsBody()->SetAngularVelocity(aV *= pow(0.05, deltaTime));
	}
}

void ForceGen::AddDrag(GameObject* object)
{
	dragGen.push_back(object);
}

void ForceGen::RemoveDrag(GameObject* object)
{
	for (int i = 0; i < dragGen.size(); i++)
	{
		if (dragGen[i] == object)
		{
			dragGen.erase(dragGen.begin() + i);
			dragGen.shrink_to_fit();
			break;
		}
	}
}

//	Getters & Setters	//


//	Constructors & Destructors	//	
ForceGen::ForceGen(glm::vec3 gravity)
{
	gForce = gravity;
}

ForceGen::~ForceGen()
{

}




//	Functions	//


//	Getters & Setters	//


//	Constructors & Destructors	//


