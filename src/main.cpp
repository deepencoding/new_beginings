#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <chrono>

#include "vendor/stb_image/stb_image.h"
#include "../headers/Shader.h"
#include "../headers/VertexBuffer.h"
#include "../headers/IndexBuffer.h"
#include "../headers/VertexArray.h"
#include "../headers/Renderer.h"
#include "../headers/Texture.h"

void processInput(GLFWwindow* win);

int main() {
	if (glfwInit() != GLFW_TRUE) {
		std::cout << "ERROR::GLFW::INITIALISATION_FAILED\n";
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Bew Neginings", 0, 0);
	if (!window) {
		std::cout << "Failed to create window.\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD.\n";
		return -1;
	}

	GLCall(glViewport(10, 10, 780, 580));
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* win, int width, int height) {
		GLCall(glViewport(10, 10, width - 20, height - 20));
		});

	// TEMPO SPACE
	float vertices1[] = {
		 // POS				 // TEXTURE COORDS
		 0.25f,  0.5f, 0.0f, 2.0f, 2.0f,
		 0.25f, -0.5f, 0.0f, 2.0f, 0.0f,
		-0.75f, -0.5f, 0.0f, 0.0f, 0.0f,
		-0.75f,  0.5f, 0.0f, 0.0f, 2.0f
	};

	float vertices2[] = {
		// POS              // COLOR
		0.25f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.25f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f,  -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	float texCoords[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.5f, 1.0f
	};

	VertexArray va;
	VertexBuffer vb(vertices1, 4 * 5 * sizeof(float));
	
	VertexBufferLayout vlayout;
	vlayout.push<float>(3);
	vlayout.push<float>(2);
	va.addVertexBuffer(vb, vlayout);
	
	IndexBuffer ib(indices, 2 * 3);

	// TEXTURES
	Texture texture1("res/textures/container.jpg");
	Texture texture2("res/textures/awesomeface.png");
	// set the texture wrapping parameters
	texture2.bind();
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	
	VertexArray va2;
	VertexBuffer vb2(vertices2, 3 * 6 * sizeof(float));

	VertexBufferLayout vlayout2;
	vlayout2.push<float>(3);
	vlayout2.push<float>(3);
	va2.addVertexBuffer(vb2, vlayout2);

	// MATH
	
	Shader shaderProg1("res/shaders/Basic1.shader");
	Shader shaderProg2("res/shaders/Basic2.shader");

	shaderProg1.bind();
	shaderProg1.setInt("oTexture1", {0});
	shaderProg1.setInt("oTexture2", {1});

	Renderer renderer;

	// RENDER LOOP
	auto t_start = std::chrono::high_resolution_clock::now();
	while (!glfwWindowShouldClose(window)) {
		// INPUT
		processInput(window);

		// MATH
		auto t_now = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
		
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(0.25f + sin((float)glfwGetTime()), 0.25f + cos((float)glfwGetTime()), 0.0f));
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		trans = glm::scale(trans, glm::vec3(0.5f + cos((float)glfwGetTime()), 0.5f + sin((float)glfwGetTime()), 0.5f));

		// RENDER
		renderer.clear();

		shaderProg1.bind();
		shaderProg1.setMat("transform", trans);
		shaderProg1.setFloat("triColor", { sin(time * 4.0f) + 1.0f / 2.0f, cos(time * 3.0f) + 0.5f / 0.6f, tan(time) * 0.5f });
		texture1.bind(0);
		texture2.bind(1);
		renderer.draw(va, ib, shaderProg1);

		trans = glm::mat4(1.0f); // reset it to identity matrix
		float scaleAmount = static_cast<float>(sin(glfwGetTime()));
		trans = glm::scale(trans, glm::vec3(scaleAmount, scaleAmount, scaleAmount));
		trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));

		shaderProg1.setMat("transform", trans);
		renderer.draw(va, ib, shaderProg1);

		renderer.draw(va2, shaderProg2);

		// CHECK AND CALL EVENTS AND SWAP BUFFERS
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* win) {
	if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(win, true);
	}
}