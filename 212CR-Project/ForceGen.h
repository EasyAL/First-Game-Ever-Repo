#pragma once
#include <vector>
#include "GameObject.h"
class ForceGen
{
private:
	//	Variables	//
	glm::vec3 gForce;
	glm::vec3 drag;

	std::vector<GameObject*> gravityGen;
	std::vector<GameObject*> dragGen;

public:
	//	Functions	//
	void UpdateForces();

	void ApplyGravity();
	void AddGravity(GameObject*);
	void RemoveGravity(GameObject*);

	void ApplyDrag(float);
	void AddDrag(GameObject*);
	void RemoveDrag(GameObject*);

	//	Constructors & Destructors	//
	ForceGen(glm::vec3);
	~ForceGen();

};

