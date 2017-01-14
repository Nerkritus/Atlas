#include "Camera.h"

using namespace Atlas;

Camera::Camera()
{
	_upVec = glm::vec3(0, 1.0f, 0);

	SetPosition(0, 1.0f, 2.0f);

	SetLookAt(0, 0, 0);

	Update();
}

void Camera::SetPosition(float x, float y, float z)
{
	_camPos = glm::vec3(x, y, z);
	_needsUpdate = true;
}

void Camera::SetLookAt(float x, float y, float z)
{
	_camTarget = glm::vec3(x, y, z);
	_needsUpdate = true;
}

void Camera::Update()
{
	if (_needsUpdate) {
		//_camDirection = glm::normalize(_camPos - _camTarget);
		//_rightVec = glm::normalize(glm::cross(_upVec, _camDirection));
		//_camUp = glm::cross(_camDirection, _rightVec);
		_viewMat = glm::lookAt(_camPos, _camTarget, _upVec);

		_needsUpdate = false;
	}
}

glm::mat4 Camera::GetViewMatrix()
{
	Update();

	return _viewMat;
}