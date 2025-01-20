#include "../headers/Camera.h"

#include <algorithm>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch )
	: m_fov(75.f), m_yaw(yaw), m_pitch(pitch),
	m_sensitivity(0.1f), m_zoom(45.f),
	m_lastX(400.f), m_lastY(300.f),
	m_firstMouse(true),
	m_camPos(position), m_camFront(glm::vec3(0.f, 0.f, -1.f)), m_camSpeed(2.5f),
	m_worldUp(up)
{
	updateCameraVectors();
}

Camera::Camera(float posx, float posy, float posz, float upx, float upy, float upz, float yaw, float pitch)
	: m_fov(75.f), m_yaw(yaw), m_pitch(pitch),
	m_sensitivity(0.1f), m_zoom(45.f),
	m_lastX(400.f), m_lastY(300.f),
	m_firstMouse(true),
	m_camPos(glm::vec3(posx, posy, posz)), m_camFront(glm::vec3(0.f, 0.f, -1.f)), m_camSpeed(2.5f),
	m_worldUp(glm::vec3(upx, upy, upz))
{
	updateCameraVectors();
}

Camera::~Camera() {}

glm::mat4 Camera::getViewMatrix() const {
	return glm::lookAt(m_camPos, m_camPos + m_camFront, m_camUp);
}

void Camera::processKeyboard(DIRECTION dir, float deltaTime)
{
	float velocity = m_camSpeed * deltaTime;
	if		(dir == DIRECTION::FORWARD)	 m_camPos += m_camFront * velocity;
	else if (dir == DIRECTION::BACKWARD) m_camPos -= m_camFront * velocity;
	else if (dir == DIRECTION::LEFT)	 m_camPos -= m_camRight * velocity;
	else if (dir == DIRECTION::RIGHT)	 m_camPos += m_camRight * velocity;
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
	xoffset *= m_sensitivity;
	yoffset *= m_sensitivity;

	m_yaw   += xoffset;
	m_yaw = glm::mod(m_yaw + xoffset, 360.f);
	m_pitch += yoffset;

	if (constrainPitch) m_pitch = std::clamp(m_pitch, -89.f, 89.f);

	updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset)
{
	m_zoom -= yoffset;
	m_zoom = std::clamp(m_zoom, 1.f, 90.f);
}

void Camera::updateCameraVectors() {
	glm::vec3 front = glm::vec3(0.f);
	front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	front.y = sin(glm::radians(m_pitch));
	front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_camFront = glm::normalize(front);

	m_camRight = glm::normalize(glm::cross(m_camFront, m_worldUp));
	m_camUp	   = glm::normalize(glm::cross(m_camRight, m_camFront));
}