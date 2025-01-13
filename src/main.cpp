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
#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"
#include "../headers/Shader.h"
#include "../headers/VertexBuffer.h"
#include "../headers/IndexBuffer.h"
#include "../headers/VertexArray.h"
#include "../headers/Renderer.h"
#include "../headers/Texture.h"

void processInput(GLFWwindow* win);

int main() {
	// ================= GLFW and OpenGL INIT ====================
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
	// ====================================================================


	//Renderer renderer;

	test::Test* currentTest = nullptr;
	test::TestMenu* testMenu = new test::TestMenu(currentTest);
	currentTest = testMenu;
	testMenu->registerTest<test::TestClearColor>("Clear Color Test1");
	testMenu->registerTest<test::TestTexture2D>("Texture Test1");

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsLight();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");

	// RENDER LOOP
	while (!glfwWindowShouldClose(window)) {
		// INPUT
		processInput(window);

		// RENDER
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		{
			static float f = 0.0f;

			ImGui::Begin("Debug");
			if (currentTest) {
				currentTest->onUpdate(0.f);
				currentTest->onRender();
				if (currentTest != testMenu && ImGui::Button("<-")) {
					delete currentTest;
					currentTest = testMenu;
				}
				currentTest->onImGuiRender();
			}
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// CHECK AND CALL EVENTS AND SWAP BUFFERS
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	delete currentTest;
	if (currentTest != testMenu) delete testMenu;

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