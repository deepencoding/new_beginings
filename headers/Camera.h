#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
public:
	Camera(GLFWwindow* win);
	~Camera();

	glm::vec3 m_camPos, m_camFront, m_camUp;
	float m_camSpeed;
	float m_fov, m_yaw, m_pitch;
	float m_lastX, m_lastY;
	bool m_firstMouse;

private:
	GLFWwindow* m_win;
};