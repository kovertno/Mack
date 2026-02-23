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