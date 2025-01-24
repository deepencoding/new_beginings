#pragma once

#include <memory>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../headers/VertexArray.h"
#include "../../headers/Shader.h"
#include "../../headers/Texture.h"
#include "../../headers/Camera.h"

#include "Test.h"

namespace test {
	class TestLighting3D : public Test
	{
	public:
		TestLighting3D(GLFWwindow* win);
		~TestLighting3D();

		void onUpdate(float deltaTime) override;
		void onRender() override;
		void onImGuiRender() override;
		void processInput(float deltaTime) override;
		void onMouse(float xpos, float ypos) override;
		void onScroll(float yoffset) override;

		std::unique_ptr<Camera> m_Camera;

	private:
		void toggleMouseCapture();

		GLFWwindow* m_win;
		mutable bool m_mouseCaptured;
		std::chrono::steady_clock::time_point m_lastToggleTime;
		const float m_cooldownTime;
		std::unique_ptr<VertexArray> m_boxVAO;
		std::unique_ptr<VertexArray> m_lightVAO;
		std::unique_ptr<VertexBuffer> m_VBO;
		std::unique_ptr<IndexBuffer> m_EBO;
		std::unique_ptr<Texture> m_earthTexture;
		std::unique_ptr<Texture> m_moonTexture;
		std::unique_ptr<Texture> m_sunTexture;
		std::unique_ptr<Shader> m_boxShaderProg;
		std::unique_ptr<Shader> m_lightShaderProg;


		glm::vec3 m_angularOffset, m_translationalOffset;
		glm::vec3 m_moonOrbitAxis;
		float m_rotationSpeed;
		glm::vec3 m_lightPos, m_lightColor;
		float m_lightDiffuseCoeff, m_lightAmbientCoeff, m_lightSpecularCoeff;
		glm::vec3 m_materialAmbient, m_materialDiffuse, m_materialSpecular;
		int m_shininessExp;
		float m_materialShininess;
		glm::mat4 m_view, m_proj, m_model;

	};
}

