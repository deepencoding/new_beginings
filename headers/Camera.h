#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum class DIRECTION {
	FORWARD, BACKWARD, LEFT, RIGHT
};

class Camera {
public:
	Camera(glm::vec3 position = glm::vec3(0.f), glm::vec3 up = glm::vec3(0.f, 1.f, 0.f), float yaw = -90.f, float pitch = 0.f);
	Camera(float posx = 0.f, float posy = 0.f, float posz = 0.f, float upx = 0.f, float upy = 1.f, float upz = 0.f, float yaw = -90.f, float pitch = 0.f);
	~Camera();

	glm::mat4 getViewMatrix() const;

	void processKeyboard(DIRECTION dir, float deltaTime);
	void processMouseMovement(float xoffset, float yoffset, bool constrainPitch);
	void processMouseScroll(float yoffset);

	float m_yaw, m_pitch;
	float m_lastX, m_lastY;
	bool m_firstMouse;
	float m_fov;
	float m_sensitivity, m_zoom;
	float m_camSpeed;
	glm::vec3 m_camPos, m_worldUp, m_camFront, m_camRight, m_camUp;

private:
	void updateCameraVectors();
};