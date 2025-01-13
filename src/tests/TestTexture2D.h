#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../headers/VertexArray.h"
#include "../../headers/Shader.h"
#include "../../headers/Texture.h"

#include "Test.h"

namespace test {
	class TestTexture2D : public Test {
	
	public:
		TestTexture2D();
		~TestTexture2D();

		void onUpdate(float deltaTime) override;
		void onRender() override;
		void onImGuiRender() override;
	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VBO;
		std::unique_ptr<IndexBuffer> m_EBO;
		std::unique_ptr<Shader> m_ShaderProg;
		std::unique_ptr<Texture> m_Texture;
		glm::mat4 m_view, m_proj, m_model;
		glm::vec3 m_angularOffset, m_translationalOffset;
		std::vector<glm::vec3> m_cubePositions;
		int frame_num;
		int x_off;
		int y_off;
	};
}