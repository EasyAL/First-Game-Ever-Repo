#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <GL/glew.h>
#include <GL/freeglut.h>

using namespace glm;

class Camera
{
private:
	mat4 modelViewMat;
	vec3 camPos;
	vec3 center;
	vec3 up;

	vec3 oldCamPos;
public:

	void LookAtPlayerSide(vec3 PlayerPosition, static unsigned int ModelViewMatLoc);
	void LookAtPlayerFront(vec3 PlayerPosition, static unsigned int ModelViewMatLoc);
	void LookAtPlayerTop(vec3 PlayerPosition, static unsigned int ModelViewMatLoc);

	mat4 GetModelViewMat();
	vec3 GetCameraPosition();
	vec3 GetCameraCenter();
	vec3 GetCameraUp();

	Camera(vec3 CameraPos, vec3 Center, vec3 Up);
	~Camera();
};

