#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// default camera values
static constexpr float FOV = 45.0f;
static constexpr float YAW = -90.0f;
static constexpr float PITCH = 0.0f;
static constexpr float MOVEMENT_SPEED = 4.0f;
static constexpr float MOUSE_SENSITIVITY = 0.1f;

class Camera {
private:
	void UpdateCameraVectors();
public:
	// camera attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// euler angles
	float Pitch;
	float Yaw;
	// camera settings
	float MovementSpeed;
	float MouseSensitivity;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float pitch = PITCH, float yaw = YAW, float movementSpeed = MOVEMENT_SPEED, float mouseSens = MOUSE_SENSITIVITY) {
		Position = position;
		Front = front;
		WorldUp = up;
		Pitch = pitch;
		Yaw = yaw;
		Pitch = pitch;
		MovementSpeed = movementSpeed;
		MouseSensitivity = mouseSens;

		UpdateCameraVectors();
	}
	~Camera() = default;

	glm::mat4 GetViewMatrix() const;
	void KeyboardMovement(Camera_Movement direction, float deltaTime);
	void MouseMovement(float xoffset, float yoffset);
};

#endif