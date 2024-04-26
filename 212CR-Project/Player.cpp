#include "Player.h"
#include "OBJloader.h"
#include <fstream>
#include <GL/glew.h>
#include <GL/freeglut.h>


Player::Player(const char* FileName, vec3 Position, float Mass)
{
	VAO = VBO = 0;
	verticesData = NULL;
	numVert = 0;

	speed = 20.0f;
	jumpHeight = 2500.0f;

	pB = new Particle(Position, Mass);
	collider = new SphereCollider(Position, 1.0f);

	CreateObject(FileName);
}

Player::~Player()
{
	free(verticesData);
}

void Player::CreateObject(const char* FileName)
{
	std::vector<VertexWithAll> mesh = loadOBJ(FileName); //Import OBJ file data into a vector array
	numVert = mesh.size();

	this->verticesData = new VertexWithAll[numVert];
	for (size_t i = 0; i < numVert; i++)
	{
		this->verticesData[i] = mesh[i];
	}
}

void Player::SetPosition(vec3 newPos)
{
	pB->SetPosition(newPos);
}

vec3 Player::GetPosition(void)
{
	return pB->GetPosition();
}


void Player::SetIDs(unsigned int vao, unsigned int vbo, unsigned int ibo)
{
	VAO = vao;
	VBO = vbo;
	IBO = ibo;
}

void Player::Setup()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexWithAll) * numVert, verticesData, GL_STATIC_DRAW);

	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexWithAll), (GLvoid*)offsetof(VertexWithAll, position));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(VertexWithAll), (GLvoid*)offsetof(VertexWithAll, normal));
	glEnableVertexAttribArray(5);

	glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, sizeof(VertexWithAll), (GLvoid*)offsetof(VertexWithAll, textcoord));
	glEnableVertexAttribArray(6);
}

void Player::updateModelMatrix(unsigned int MatrixLoc, Camera* camera)
{
	modelMatrix = mat4(1.0);
	modelMatrix = lookAt(camera->GetCameraPosition(), camera->GetCameraCenter(), camera->GetCameraUp()); //camera matrix, apply first
	//modelMatrix = scale(modelMatrix, vec3(1.0f, 1.0f, 1.0f));  //scale down the model
	modelMatrix = translate(modelMatrix, pB->GetPosition());
	glUniformMatrix4fv(MatrixLoc, 1, GL_FALSE, value_ptr(modelMatrix));  //send modelview matrix to the shader
}

void Player::Draw()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, numVert);
}

void Player::Update(float DeltaTime)
{
	//  Controls    //
	if (GameObject::keys['a'] == true || GameObject::keys['A'] == true)
		pB->AddForce(glm::vec3(-1.0f, 0.0f, 0.0f) * speed, glm::vec3(0.0f, 0.0f, 0.0f));
	
	if (GameObject::keys[' '] == true)
	{
		glm::vec3 tempV = pB->GetVelocity();
		tempV.y = 0.0f;
		pB->SetVelocity(tempV);
		pB->AddForce(glm::vec3(0.0f, 1.0f, 0.0f) * jumpHeight, glm::vec3(0.0f, 0.0f, 0.0f));
		GameObject::keys[' '] = false;
	}

	if (GameObject::keys['d'] == true || GameObject::keys['D'] == true)
		pB->AddForce(glm::vec3(1.0f, 0.0f, 0.0f) * speed, glm::vec3(0.0f, 0.0f, 0.0f));


	pB->Update(DeltaTime, integrationMethod);	//	Implementing the changes caused by the forces above	//


	collider->SetPosition(pB->GetPosition());
}