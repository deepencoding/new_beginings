#include "../../headers/Renderer.h"

#include "../vendor/imgui/imgui.h"
#include "../vendor/imgui/imgui_impl_glfw.h"
#include "../vendor/imgui/imgui_impl_opengl3.h"
#include "TestLighting3D.h"

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);



test::TestLighting3D::TestLighting3D(GLFWwindow* win)
	: m_win(win), m_mouseCaptured(false), m_lastToggleTime(std::chrono::steady_clock::now()), m_cooldownTime(0.3f),
	m_view(1.f),
	m_proj(1.f),
	m_model(1.f),
	m_angularOffset(0.f, 0.f, 0.f),
	m_moonOrbitAxis(-0.2f, 1.f, -1.f),
	m_rotationSpeed(0.1f),
	m_lightPos(0.6f, 0.5f, 1.f), m_lightColor(1.f),
	m_lightDiffuseCoeff(0.5f), m_lightAmbientCoeff(0.2f), m_lightSpecularCoeff(1.f),
	m_materialAmbient(1.0f, 0.5f, 0.31f),
	m_materialDiffuse(1.0f, 0.5f, 0.31f),
	m_materialSpecular(0.5f, 0.5f, 0.5f),
	m_materialShininess(32.f), m_shininessExp(1)
{
	float vertices[] = {
		// (top-left - clockwise)
		// Coords            // normal       // texture coords
		// Back Face
		-0.5f,  0.5f, -0.5f, 0.f, 0.f, -1.f, 1.f,   0.667f,
		 0.5f,  0.5f, -0.5f, 0.f, 0.f, -1.f, 0.75f, 0.667f,
		 0.5f, -0.5f, -0.5f, 0.f, 0.f, -1.f, 0.75f, 0.334f,
		-0.5f, -0.5f, -0.5f, 0.f, 0.f, -1.f, 1.f,   0.334f,

		// Front Face
		-0.5f,  0.5f,  0.5f, 0.f, 0.f, 1.f, 0.25f, 0.667f,
		 0.5f,  0.5f,  0.5f, 0.f, 0.f, 1.f, 0.5f,  0.667f,
		 0.5f, -0.5f,  0.5f, 0.f, 0.f, 1.f, 0.5f,  0.334f,
		-0.5f, -0.5f,  0.5f, 0.f, 0.f, 1.f, 0.25f, 0.334f,

		// Left Face
		-0.5f,  0.5f, -0.5f, -1.f, 0.f, 0.f, 1.f,   0.667f,
		-0.5f,  0.5f,  0.5f, -1.f, 0.f, 0.f, 0.75f, 0.667f,
		-0.5f, -0.5f,  0.5f, -1.f, 0.f, 0.f, 0.75f, 0.334f,
		-0.5f, -0.5f, -0.5f, -1.f, 0.f, 0.f, 1.f,   0.334f,

		// Right Face
		 0.5f,  0.5f,  0.5f, 1.f, 0.f, 0.f, 0.f,   0.667f,
		 0.5f,  0.5f, -0.5f, 1.f, 0.f, 0.f, 0.25f, 0.667f,
		 0.5f, -0.5f, -0.5f, 1.f, 0.f, 0.f, 0.25f, 0.334f,
		 0.5f, -0.5f,  0.5f, 1.f, 0.f, 0.f, 0.f,   0.334f,

		 // Down Face
		 0.5f, -0.5f,  0.5f, 0.f, -1.f, 0.f, 0.25f, 0.334f,
		-0.5f, -0.5f,  0.5f, 0.f, -1.f, 0.f, 0.5f,  0.334f,
		-0.5f, -0.5f, -0.5f, 0.f, -1.f, 0.f, 0.5f,  0.f,
		 0.5f, -0.5f, -0.5f, 0.f, -1.f, 0.f, 0.25f, 0.f,

		 // Up Face
		 0.5f,  0.5f, -0.5f, 0.f, 1.f, 0.f, 0.25f, 1.f,
		-0.5f,  0.5f, -0.5f, 0.f, 1.f, 0.f, 0.5f,  1.f,
		-0.5f,  0.5f,  0.5f, 0.f, 1.f, 0.f, 0.5f,  0.667f,
		 0.5f,  0.5f,  0.5f, 0.f, 1.f, 0.f, 0.25f, 0.667f,
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

	m_boxVAO = std::make_unique<VertexArray>();
	m_VBO = std::make_unique<VertexBuffer>(vertices, static_cast<unsigned int>(1ULL * 6 * 4 * 8 * sizeof(float)));
	VertexBufferLayout vlayout;
	vlayout.push<float>(3);
	vlayout.push<float>(3);
	vlayout.push<float>(2);
	m_boxVAO->addVertexBuffer(*m_VBO, vlayout);
	m_EBO = std::make_unique<IndexBuffer>(indices, 6 * 6);
	
	m_lightVAO = std::make_unique<VertexArray>();
	m_lightVAO->addVertexBuffer(*m_VBO, vlayout);
	m_EBO->bind();

	toggleMouseCapture();
	m_Camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));

	m_earthTexture = std::make_unique<Texture>("res/textures/earth_tex.png");
	m_sunTexture = std::make_unique<Texture>("res/textures/sun_tex.png");

	m_boxShaderProg = std::make_unique<Shader>("res/shaders/Basic2.shader");
	m_boxShaderProg->bind();
	m_boxShaderProg->setInt("earthTex", { 1 });

	m_boxShaderProg->setMat("model", m_model);

	m_lightShaderProg = std::make_unique<Shader>("res/shaders/lightingShader.shader");
	m_lightShaderProg->bind();
	m_lightShaderProg->setInt("sunTex", { 1 });

}

test::TestLighting3D::~TestLighting3D()
{
	toggleMouseCapture();
}

void test::TestLighting3D::onUpdate(float deltaTime)
{
}

void test::TestLighting3D::onRender()
{
	Renderer renderer;
	renderer.clear({ 0.f, 0.f, 0.f, 0.f });

	float glfwTime = (float)glfwGetTime();
	glm::mat4 rotationMat = glm::rotate(glm::mat4(1.f), glm::radians(m_rotationSpeed), m_moonOrbitAxis);
	m_lightPos = glm::vec3(rotationMat * glm::vec4(m_lightPos, 1.f));

	m_model = glm::mat4(1.f);
	m_model = glm::translate(m_model, m_lightPos);
	m_model = glm::scale(m_model, glm::vec3(0.2f));

	m_proj = glm::perspective(glm::radians(m_Camera->m_zoom), 800.f / 600.f, 0.1f, 100.f);
	m_view = m_Camera->getViewMatrix();

	m_boxShaderProg->bind();
	glm::vec3 specularColor = m_lightColor * m_lightSpecularCoeff;
	glm::vec3 diffuseColor = m_lightColor * m_lightDiffuseCoeff;
	glm::vec3 ambientColor = diffuseColor * m_lightAmbientCoeff;

	m_boxShaderProg->setVec("light.position", m_lightPos);
	m_boxShaderProg->setVec("light.ambient", ambientColor);
	m_boxShaderProg->setVec("light.diffuse", diffuseColor);
	m_boxShaderProg->setVec("light.specular", specularColor);

	m_boxShaderProg->setVec("material.ambient", m_materialAmbient);
	m_boxShaderProg->setVec("material.diffuse", m_materialDiffuse);
	m_boxShaderProg->setVec("material.specular", m_materialSpecular);
	m_materialShininess = static_cast<float>(1 << m_shininessExp);
	m_boxShaderProg->setFloat("material.shininess", { m_materialShininess });

	m_boxShaderProg->setVec("viewPos", m_Camera->m_camPos);
	m_boxShaderProg->setMat("proj", m_proj);
	m_boxShaderProg->setMat("view", m_view);
	m_earthTexture->bind(1);
	renderer.draw(*m_boxVAO, *m_EBO, *m_boxShaderProg);
	
	m_lightShaderProg->bind();
	m_lightShaderProg->setMat("proj", m_proj);
	m_lightShaderProg->setMat("view", m_view);
	m_lightShaderProg->setMat("model", m_model);
	m_sunTexture->bind(1);
	renderer.draw(*m_lightVAO, *m_EBO, *m_lightShaderProg);
}

void test::TestLighting3D::onImGuiRender()
{
	ImGui::Text("Rotation Stuff");
	ImGui::SliderFloat("Rotation Speed", &m_rotationSpeed, 0.f, 1.f);
	ImGui::SliderFloat3("Moon Orbit Axis", &m_moonOrbitAxis.x, -1.f, 1.f);
	ImGui::Text("Light Stuff");
	ImGui::ColorEdit3("Light Color", &m_lightColor.x);
	ImGui::SliderFloat("Light Ambient Intensity", &m_lightAmbientCoeff, 0.f, 1.f);
	ImGui::SliderFloat("Light Diffuse Intensity", &m_lightDiffuseCoeff, 0.f, 1.f);
	ImGui::SliderFloat("Light Specular Intensity", &m_lightSpecularCoeff, 0.f, 1.f);
	ImGui::Text("Material Stuff");
	ImGui::ColorEdit3("Material Ambient Color", &m_materialAmbient.x);
	ImGui::ColorEdit3("Material Diffuse Color", &m_materialDiffuse.x);
	ImGui::ColorEdit3("Material Specular Color", &m_materialSpecular.x);
	ImGui::SliderInt("Shininess Factor Exponent", &m_shininessExp, 1, 10);
}

void test::TestLighting3D::processInput(float deltaTime)
{
	if (glfwGetKey(m_win, GLFW_KEY_W) == GLFW_PRESS)
		m_Camera->processKeyboard(DIRECTION::FORWARD, deltaTime);
	if (glfwGetKey(m_win, GLFW_KEY_S) == GLFW_PRESS)
		m_Camera->processKeyboard(DIRECTION::BACKWARD, deltaTime);
	if (glfwGetKey(m_win, GLFW_KEY_A) == GLFW_PRESS)
		m_Camera->processKeyboard(DIRECTION::LEFT, deltaTime);
	if (glfwGetKey(m_win, GLFW_KEY_D) == GLFW_PRESS)
		m_Camera->processKeyboard(DIRECTION::RIGHT, deltaTime);
	if (glfwGetKey(m_win, GLFW_KEY_M) == GLFW_PRESS)
		toggleMouseCapture();
}

void test::TestLighting3D::onMouse(float xpos, float ypos)
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

void test::TestLighting3D::onScroll(float yoffset)
{
	m_Camera->processMouseScroll(yoffset);
}

void test::TestLighting3D::toggleMouseCapture()
{
	auto now = std::chrono::steady_clock::now();
	float elapsed = std::chrono::duration<float>(now - m_lastToggleTime).count();
	if (elapsed < m_cooldownTime) return;

	if (m_mouseCaptured) {
		glfwSetCursorPosCallback(m_win, ImGui_ImplGlfw_CursorPosCallback);
		glfwSetScrollCallback(m_win, ImGui_ImplGlfw_ScrollCallback);
		glfwSetInputMode(m_win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
		m_Camera->m_firstMouse = true;
	} else {
		glfwSetCursorPosCallback(m_win, mouse_callback);
		glfwSetScrollCallback(m_win, scroll_callback);
		glfwSetInputMode(m_win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
	}
	m_mouseCaptured = !m_mouseCaptured;
	m_lastToggleTime = now;
}
