#include "Camera.h"

mat4 Camera::GetModelViewMat()
{
	return modelViewMat;
}

vec3 Camera::GetCameraPosition()
{
	return camPos;
}

vec3 Camera::GetCameraCenter()
{
	return center;
}

vec3 Camera::GetCameraUp()
{
	return up;
}


void Camera::LookAtPlayerSide(vec3 PlayerPosition, static unsigned int ModelViewMatLoc)
{
	camPos.x = PlayerPosition.x;
	center.x = PlayerPosition.x;

	modelViewMat = mat4(1.0);
	modelViewMat = lookAt(camPos, center, up);	//	Change camera pos
	glUniformMatrix4fv(ModelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
}

void Camera::LookAtPlayerFront(vec3 PlayerPosition, static unsigned int ModelViewMatLoc)
{
	center.x = PlayerPosition.x;
	camPos.x = PlayerPosition.x + oldCamPos.x;

	center.y = PlayerPosition.y;
	camPos.y = PlayerPosition.y + oldCamPos.y;

	modelViewMat = mat4(1.0);
	modelViewMat = lookAt(camPos, center, up);	//	Change camera pos
	glUniformMatrix4fv(ModelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
}

void Camera::LookAtPlayerTop(vec3 PlayerPosition, static unsigned int ModelViewMatLoc)
{
	center.x = PlayerPosition.x;
	camPos.x = PlayerPosition.x;

	

	modelViewMat = mat4(1.0);
	modelViewMat = lookAt(camPos, center, up);	//	Change camera pos
	glUniformMatrix4fv(ModelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
}



Camera::Camera(vec3 CameraPos, vec3 Center, vec3 Up)
{
	modelViewMat = mat4(1.0);
	camPos = CameraPos;
	center = Center;
	up = Up;
	oldCamPos = CameraPos;
}

Camera::~Camera()
{

}