#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include "vertex.h"
#include "GameObject.h"

using namespace glm;

class Player : public GameObject
{
private:
	int numVert;
	float speed;
	float jumpHeight;

	VertexWithAll* verticesData;  //Data with vertices, normal, texCoords

	unsigned int VAO;
	unsigned int VBO;
	unsigned int IBO; //for triangle indices buffer

	mat4 modelMatrix;

	void CreateObject(const char* FileName);
public:


	Player(const char* FileName, vec3 Position, float Mass);
	~Player();

	void SetPosition(vec3 newPos);
	vec3 GetPosition(void);

	void SetIDs(unsigned int, unsigned int, unsigned int);
	void updateModelMatrix(unsigned int MatrixLoc, Camera* Camera);

	void Setup();
	void Draw();
	void Update(float DeltaTime);
};
