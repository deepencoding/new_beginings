#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <chrono>
#include <vector>

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"
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
	GLCall(glEnable(GL_DEPTH_TEST));

	// TEMPO SPACE
	float vertices1[] = {
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

	VertexArray va;
	VertexBuffer vb(vertices1, 4 * 6 * 7 * sizeof(float));
	IndexBuffer ib(indices, 6 * 6);
	
	VertexBufferLayout vlayout;
	vlayout.push<float>(3);
	vlayout.push<float>(2);
	vlayout.push<float>(2);
	va.addVertexBuffer(vb, vlayout);

	// TEXTURES
	/*Texture texture1("res/textures/container.jpg");
	Texture texture2("res/textures/awesomeface.png");*/
	Texture texture3("res/textures/applause-leo.png");
	// set the texture wrapping parameters
	//texture2.bind();
	//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

	// MATH
	glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
	glm::mat4 proj = glm::perspective(glm::radians(75.0f), 800.f / 600.f, 0.1f, 100.f);

	std::vector<glm::vec3> cubePositions {
		glm::vec3( 0.0f,  0.0f,  0.0f),
		glm::vec3( 2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3( 2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3( 1.3f, -2.0f, -2.5f),
		glm::vec3( 1.5f,  2.0f, -2.5f),
		glm::vec3( 1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	Shader shaderProg1("res/shaders/Basic1.shader");

	shaderProg1.bind();
	/*shaderProg1.setInt("oTexture1", {0});
	shaderProg1.setInt("oTexture2", {1});*/
	shaderProg1.setInt("oTexture3", {2});
	
	texture3.bind(2);
	shaderProg1.setMat("view", view);
	shaderProg1.setMat("proj", proj);

	Renderer renderer;
	int frame_num = 0;
	int x_off = 0;
	int y_off = 0;

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsLight();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");

	glm::vec3 angular_offset (0.f, 0.f, 0.f);
	glm::vec3 translational_offset (0.f, 0.f, 0.f);
	

	// RENDER LOOP
	//auto t_start = std::chrono::high_resolution_clock::now();
	while (!glfwWindowShouldClose(window)) {
		// INPUT
		processInput(window);

		// MATH
		/*auto t_now = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
		*/
		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// RENDER
		renderer.clear();

		shaderProg1.bind();
		shaderProg1.setInt("x_offset", { x_off });
		shaderProg1.setInt("y_offset", { y_off });
		frame_num = (frame_num + 1) % 50;
		if (!frame_num) {
			x_off = ((x_off + 1) % 5);
			if (!x_off) {
				y_off = ((y_off + 1) % 5);
			}
		}
		
		for (unsigned i = 0; i < 10; ++i) {
			model = glm::translate(glm::mat4(1.f), cubePositions[i] + translational_offset);
			float angle = 20.f * i;
			model = glm::rotate(model, glm::radians(angular_offset.x), glm::vec3(1.f, 0.f, 0.f));
			model = glm::rotate(model, glm::radians(angular_offset.y), glm::vec3(0.f, 1.f, 0.f));
			model = glm::rotate(model, glm::radians(angular_offset.z), glm::vec3(0.f, 0.f, 1.f));
			shaderProg1.setMat("model", model);
			renderer.draw(va, ib, shaderProg1);
		}

		{
			static float f = 0.0f;

			ImGui::Begin("Hello, world!");
			ImGui::SliderFloat3("angular_offsets", &angular_offset.x, 0.0f, 360.0f);
			ImGui::SliderFloat3("translational_offsets", &translational_offset.x, -1.0f, 1.0f);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// CHECK AND CALL EVENTS AND SWAP BUFFERS
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

void processInput(GLFWwindow* win) {
	if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(win, true);
	}
}