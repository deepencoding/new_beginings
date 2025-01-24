#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../headers/VertexArray.h"
#include "../../headers/Shader.h"
#include "../../headers/Texture.h"
#include "../../headers/Camera.h"

#include "Test.h"

namespace test {
	class TestTexture2D : public Test {
	
	public:
		TestTexture2D(GLFWwindow* win);
		~TestTexture2D();

		void onUpdate(float deltaTime) override;
		void onRender() override;
		void onImGuiRender() override;
		void processInput(float deltaTime) override;
		void onMouse(float xpos, float ypos) override;
		void onScroll(float yoffset) override;
		std::unique_ptr<Camera> m_Camera;

	private:
		GLFWwindow* m_win;
		int frame_num, x_off, y_off;
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VBO;
		std::unique_ptr<IndexBuffer> m_EBO;
		std::unique_ptr<Shader> m_ShaderProg;
		std::unique_ptr<Texture> m_Texture;
		glm::mat4 m_view, m_proj, m_model;
		glm::vec3 m_angularOffset, m_translationalOffset;
		std::vector<glm::vec3> m_cubePositions;
	};
}