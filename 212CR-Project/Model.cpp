#include "Model.h"
#include "OBJloader.h"
#include <fstream>
#include <GL/glew.h>
#include <GL/freeglut.h>

Model::Model(const char* FileName, vec3 Position, float Mass, std::string Type)
{
	VAO = VBO = 0;
	verticesData = NULL;
	numVert = 0;

	pB = new Particle(Position, Mass);
	type = Type;

	CreateObject(FileName);
}

Model::Model(const char* FileName, vec3 Position, float Mass, std::string Type, std::string ColliderType)
{
	VAO = VBO = 0;
	verticesData = NULL;
	numVert = 0;

	type = Type;
	pB = new Particle(Position, Mass);

	if (ColliderType == "Sphere" || ColliderType == "sphere" || ColliderType == "SPHERE")
	{
		collider = new SphereCollider(Position, 3.0f);
	}

	CreateObject(FileName);
}

Model::~Model()
{
	free(verticesData);
}

void Model::CreateObject(const char* FileName)
{
	std::vector<VertexWithAll> mesh = loadOBJ(FileName); //Import OBJ file data into a vector array
	numVert = mesh.size();
	
	this->verticesData = new VertexWithAll[numVert];
	for (size_t i = 0; i < numVert; i++)
	{
		this->verticesData[i] = mesh[i];
	}
}

std::string Model::GetType()
{
	return type;
}

void Model::SetPosition(vec3 newPos)
{
	pB->SetPosition(newPos);
}

vec3 Model::GetPosition(void)
{
	return pB->GetPosition();
}


void Model::SetIDs(unsigned int vao, unsigned int vbo, unsigned int ibo)
{
	VAO = vao;
	VBO = vbo;
	IBO = ibo;
}

void Model::Setup()
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

void Model::updateModelMatrix(unsigned int MatrixLoc, Camera* camera)
{
	modelMatrix = mat4(1.0);
	modelMatrix = lookAt(camera->GetCameraPosition(), camera->GetCameraCenter(), camera->GetCameraUp()); //camera matrix, apply first
	//modelMatrix = scale(modelMatrix, vec3(0.f, 0.f, 0.f));  //scale down the model
	modelMatrix = translate(modelMatrix, GetPosition());
	glUniformMatrix4fv(MatrixLoc, 1, GL_FALSE, value_ptr(modelMatrix));  //send modelview matrix to the shader
}

void Model::Draw()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, numVert);
}

void Model::Update(float deltaTime)
{




	
	
	pB->Update(deltaTime, integrationMethod);	//	Implementing the changes caused by the forces above	//
	if (collider != nullptr)
	{
		collider->SetPosition(pB->GetPosition());
	}
}