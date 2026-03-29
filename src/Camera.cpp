#include "Camera.hpp"

glm::mat4 Camera::GetViewMatrix() const{
	return glm::lookAt(Position, Position + Front, Up);
}

void Camera::KeyboardMovement(Camera_Movement direction, float deltaTime) {
	float velocity = MovementSpeed * deltaTime;
	if (direction == FORWARD) {
		Position += glm::vec3(Front.x, Front.y, Front.z) * velocity;
	}
	if (direction == BACKWARD) {
		Position -= glm::vec3(Front.x, Front.y, Front.z) * velocity;
	}
	if (direction == LEFT) {
		Position -= glm::vec3(Right.x, Right.y, Right.z) * velocity;
	}
	if (direction == RIGHT) {
		Position += glm::vec3(Right.x, Right.y, Right.z) * velocity;
	}
}

void Camera::MouseMovement(float xoffset, float yoffset) {
	Yaw += xoffset * MouseSensitivity;
	Pitch += yoffset * MouseSensitivity;

	if (Pitch >= 89.0f)
		Pitch = 89.0f;
	if (Pitch <= -89.0f)
		Pitch = -89.0f;

	// update the vectors using new euler angles
	UpdateCameraVectors();
}

void Camera::Orbit(float radius, float angle, float deltaTime) {
	float x = 0.0f + radius * cos(angle);
	float z = 0.0f + radius * sin(angle);
	Position = glm::vec3(x, 8.0f, z);
	Front = glm::normalize(0.0f - Position);
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));

	/* 
	CALCULATED BY DERIVING FROM THIS:
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	*/
	Yaw = glm::degrees(atan2(Front.z, Front.x));
	Pitch = glm::degrees(asin(Front.y));
}

void Camera::UpdateCameraVectors() {
	glm::vec3 front;
	// calculate the new front vector
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);
	// also recalculate Right and Up vectors
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}