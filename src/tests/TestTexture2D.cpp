#include "../../headers/util.h"
#include "../vendor/imgui/imgui.h"
#include "TestTexture2D.h"

namespace test {
	TestTexture2D::TestTexture2D() {
		
	}
	TestTexture2D::~TestTexture2D() {}

	void TestTexture2D::onUpdate(float deltaTime) {}
	void TestTexture2D::onRender() {
		GLCall(glClearColor(0.f, 0.f, 0.f, 0.f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}
	void TestTexture2D::onImGuiRender() {
	}
}