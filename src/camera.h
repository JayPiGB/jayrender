#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

enum CAMERA_MOVEMENT {
	UP,
	DOWN,
	FRONT,
	BACK,
	LEFT,
	RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float ROW = 0.0f;
const float SPEED = 25.0f;
const float ZOOM = 0.0f;

class Camera {
public:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;

	float yaw;
	float pitch;
	float row;

	float movementSpeed;
	float mouseSensitivity;
	float zoom;

	Camera(float x, float y, float z) :
		position(x, y, z),
		front(0.0f, 0.0f, -1.0f),
		up(0.0f, 1.0f, 0.0f),
		yaw(YAW),
		pitch(PITCH),
		row(ROW),
		movementSpeed(SPEED),
		mouseSensitivity(0.1f),
		zoom(ZOOM) {}

	glm::mat4 getViewMatrix() const {
		return glm::lookAt(position, position + front, up);
	}

	void processMouseMovement(float xoffset, float yoffset) {
		xoffset *= mouseSensitivity;
		yoffset *= mouseSensitivity;

		yaw += xoffset;
		pitch -= yoffset;

		if (pitch > 89.0f) {
			pitch = 89.0f;
		}
		if (pitch < -89.0f) {
			pitch = -89.0f;
		}

		updateCameraFront();
	}

	void moveCamera(CAMERA_MOVEMENT movement, float deltaTime) {
		float cameraSpeed = movementSpeed * deltaTime;
		glm::vec3 movementFront = front;
		movementFront.y = 0.0f;

		switch (movement) {
		case FRONT:
			position += cameraSpeed * movementFront;
			break;
			
		case BACK:
			position -= cameraSpeed * movementFront;
			break;

		case LEFT:
			position -= cameraSpeed * glm::normalize(glm::cross(movementFront, up));
			break;

		case RIGHT:
			position += cameraSpeed * glm::normalize(glm::cross(movementFront, up));
			break;

		case UP:
			position += cameraSpeed * glm::normalize(up);
			break;

		case DOWN:
			position -= cameraSpeed * glm::normalize(up);
		}
	}

	void centerCamera() {
		glm::vec3 worldOrigin = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 direction = worldOrigin - position;

		direction = glm::normalize(direction);

		pitch = glm::degrees(glm::asin(direction.y));
		yaw = glm::degrees(glm::atan(direction.z, direction.x));

		updateCameraFront();
	}

private:
	void updateCameraFront() {
		glm::vec3 direction{};
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

		front = glm::normalize(direction);
	}
};

