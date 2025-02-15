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
#include "tests/TestLighting3D.h"

#include "../headers/Shader.h"
#include "../headers/VertexBuffer.h"
#include "../headers/IndexBuffer.h"
#include "../headers/VertexArray.h"
#include "../headers/Renderer.h"
#include "../headers/Texture.h"

static void processInput(GLFWwindow* win, test::Test* currentTest, float deltaTime);

test::Test* currentTest = nullptr;

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

	GLCall(glViewport(0, 0, 800, 600));
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* win, int width, int height) { GLCall(glViewport(0, 0, width, height)); });
	GLCall(glEnable(GL_DEPTH_TEST));

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//ImGui::StyleColorsLight();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");

	// ====================================================================

	test::TestMenu* testMenu = new test::TestMenu(currentTest, window);
	currentTest = testMenu;
	testMenu->registerTest<test::TestClearColor>("Clear Color Test1");
	testMenu->registerTest<test::TestTexture2D>("Texture Test1");
	testMenu->registerTest<test::TestLighting3D>("Lighting Test1");

	float deltaTime = 0.f;
	float lastFrame = 0.f;

	// RENDER LOOP
	while (!glfwWindowShouldClose(window)) {
		// INPUT
		processInput(window, currentTest, deltaTime);


		// RENDER
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		{
			ImGui::Begin("Debug");
			if (currentTest) {
				currentTest->onUpdate(deltaTime);
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

static void processInput(GLFWwindow* win, test::Test* currentTest, float deltaTime) {
	if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(win, true);
	}

	if (currentTest) currentTest->processInput(deltaTime);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	if (currentTest) currentTest->onScroll(static_cast<float>(yoffset));
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (currentTest) currentTest->onMouse(static_cast<float>(xpos), static_cast<float>(ypos));
}