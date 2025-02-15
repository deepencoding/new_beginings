#include "../../headers/util.h"
#include <GLFW/glfw3.h>
#include "../../headers/Renderer.h"


#include "../vendor/imgui/imgui.h"
#include "TestTexture2D.h"

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


namespace test {
	TestTexture2D::TestTexture2D(GLFWwindow* win)
		: m_win(win), frame_num(0), x_off(0), y_off(0),
		m_angularOffset(0.f, 0.f, 0.f), m_translationalOffset(0.f, 0.f, 0.f),
		m_view(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f))), 
		m_proj(glm::perspective(glm::radians(75.f), 800.f / 600.f, 0.1f, 100.f)),
		m_model(glm::rotate(glm::mat4(1.0f), glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f)))

	{
		float vertices[] = {
			// Back Face (bottom-left - anti-clockwise)
			-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.834f,
			 0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.2f, 0.834f,
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.2f,   1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,   1.0f,

			// Front Face (bottom-left - anticlockwise)
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.834f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.2f, 0.834f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.2f,   1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,   1.0f,

			// Left Face
			-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 0.834f,
			-0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.2f, 0.834f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.2f,   1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,   1.0f,

			// Right Face
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 0.834f,
			 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.2f, 0.834f,
			 0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.2f,   1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,   1.0f,


			 // Down Face
			 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.834f,
			  0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.2f, 0.834f,
			  0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.2f,   1.0f,
			 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,   1.0f,

			 // Up Face
			 -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.834f,
			  0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.2f, 0.834f,
			  0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.2f,   1.0f,
			 -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,   1.0f,
		};

		unsigned int indices[] = {
			// Back face
			0, 1, 2,
			2, 3, 0,

			// Front face
			4, 5, 6,
			6, 7, 4,

			// Left face
			8, 9, 10,
			10, 11, 8,

			// Right face
			12, 13, 14,
			14, 15, 12,

			// Down face
			16, 17, 18,
			18, 19, 16,

			// Up face
			20, 21, 22,
			22, 23, 20,
		};

		m_VAO = std::make_unique<VertexArray>();
		m_VBO = std::make_unique<VertexBuffer>(vertices, static_cast<unsigned int>(1ULL * 4 * 6 * 7 * sizeof(float)));

		VertexBufferLayout vlayout;
		vlayout.push<float>(3);
		vlayout.push<float>(2);
		vlayout.push<float>(2);
		m_VAO->addVertexBuffer(*m_VBO, vlayout);
		m_EBO = std::make_unique<IndexBuffer>(indices, 6 * 6);

		m_Texture = std::make_unique<Texture>("res/textures/applause-leo.png");

		m_cubePositions = {
			glm::vec3( 0.0f,  0.0f,  0.0f ),
			glm::vec3( 2.0f,  5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f ),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3( 2.4f, -0.4f, -3.5f ),
			glm::vec3(-1.7f,  3.0f, -7.5f ),
			glm::vec3( 1.3f, -2.0f, -2.5f ),
			glm::vec3( 1.5f,  2.0f, -2.5f ),
			glm::vec3( 1.5f,  0.2f, -1.5f ),
			glm::vec3(-1.3f,  1.0f, -1.5f ),
		};

		glfwSetCursorPosCallback(m_win, mouse_callback);
		glfwSetScrollCallback(m_win, scroll_callback);
		glfwSetInputMode(m_win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		m_Camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));


		m_ShaderProg = std::make_unique<Shader>("res/shaders/Basic1.shader");
		m_ShaderProg->bind();
		m_ShaderProg->setInt("oTexture3", { 2 });
		m_ShaderProg->setMat("view", m_view);
		m_ShaderProg->setMat("proj", m_proj);
	}

	TestTexture2D::~TestTexture2D() {}

	void TestTexture2D::onUpdate(float deltaTime) {
		//m_Camera->m_camSpeed = 2.5f * deltaTime;
	}

	void TestTexture2D::onRender() {
		frame_num = (frame_num + 1) % 50;
		if (!frame_num) {
			x_off = ((x_off + 1) % 5);
			if (!x_off) {
				y_off = ((y_off + 1) % 5);
			}
		}

		m_view = glm::lookAt(m_Camera->m_camPos, m_Camera->m_camPos + m_Camera->m_camFront, m_Camera->m_camUp);
		m_proj = glm::perspective(glm::radians(m_Camera->m_zoom), 800.f / 600.f, 0.1f, 100.f);

		Renderer renderer;
		renderer.clear();

		m_Texture->bind(2);
		m_ShaderProg->bind();
		m_ShaderProg->setInt("x_offset", { x_off });
		m_ShaderProg->setInt("y_offset", { y_off });
		m_ShaderProg->setMat("view", m_view);
		m_ShaderProg->setMat("proj", m_proj);

		for (unsigned i = 0; i < 10; ++i) {
			m_model = glm::translate(glm::mat4(1.f), m_cubePositions[i] + m_translationalOffset);
			float angle = 20.f * i;
			m_model = glm::rotate(m_model, glm::radians(m_angularOffset.x), glm::vec3(1.f, 0.f, 0.f));
			m_model = glm::rotate(m_model, glm::radians(m_angularOffset.y), glm::vec3(0.f, 1.f, 0.f));
			m_model = glm::rotate(m_model, glm::radians(m_angularOffset.z), glm::vec3(0.f, 0.f, 1.f));
			m_ShaderProg->setMat("model", m_model);
			renderer.draw(*m_VAO, *m_EBO, *m_ShaderProg);
		}
	}

	void TestTexture2D::onImGuiRender() {
		ImGui::SliderFloat3("Angular Offset", &m_angularOffset.x, 0.0f, 360.0f);
		ImGui::SliderFloat3("Translational Offset", &m_translationalOffset.x, -1.0f, 1.0f);
	}

	void TestTexture2D::processInput(float deltaTime)
	{
		if (glfwGetKey(m_win, GLFW_KEY_W) == GLFW_PRESS)
			m_Camera->processKeyboard(DIRECTION::FORWARD, deltaTime);
		if (glfwGetKey(m_win, GLFW_KEY_S) == GLFW_PRESS)
			m_Camera->processKeyboard(DIRECTION::BACKWARD, deltaTime);
		if (glfwGetKey(m_win, GLFW_KEY_A) == GLFW_PRESS)
			m_Camera->processKeyboard(DIRECTION::LEFT, deltaTime);
		if (glfwGetKey(m_win, GLFW_KEY_D) == GLFW_PRESS)
			m_Camera->processKeyboard(DIRECTION::RIGHT, deltaTime);
	}

	void TestTexture2D::onMouse(float xpos, float ypos)
	{
		if (m_Camera->m_firstMouse) {
			m_Camera->m_lastX = (float)xpos;
			m_Camera->m_lastY = (float)ypos;
			m_Camera->m_firstMouse = false;
		}

		float xoffset = (float)xpos - m_Camera->m_lastX;
		float yoffset = m_Camera->m_lastY - (float)ypos;
		m_Camera->m_lastX = (float)xpos;
		m_Camera->m_lastY = (float)ypos;

		m_Camera->processMouseMovement(xoffset, yoffset, true);
	}

	void TestTexture2D::onScroll(float yoffset)
	{
		m_Camera->processMouseScroll(yoffset);
	}

};