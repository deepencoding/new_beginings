#include "../headers/Camera.h"

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


Camera::Camera(GLFWwindow* win)
	: m_win(win), m_fov(75.f), m_yaw(-90.f), m_pitch(0.f),
	m_lastX(400.f), m_lastY(300.f),
	m_firstMouse(true),
	m_camPos(glm::vec3(0.f, 0.f, 3.f)), m_camFront(glm::vec3(0.f, 0.f, -1.f)), m_camUp(glm::vec3(0.f, 1.f, 0.f)), m_camSpeed(2.5f)
{
	glfwSetCursorPosCallback(m_win, mouse_callback);
	glfwSetScrollCallback(m_win, scroll_callback);
	glfwSetInputMode(m_win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glm::vec3 cameraTarget = glm::vec3(0.f, 0.f, 0.f);

	cameraTarget.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	cameraTarget.y = sin(glm::radians(m_pitch));
	cameraTarget.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

	glm::vec3 cameraDir = glm::normalize(m_camPos - cameraTarget);

	glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDir));
	glm::vec3 cameraUp = glm::cross(cameraDir, cameraRight);
}

Camera::~Camera() {}