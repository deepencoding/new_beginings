#pragma once

#include "Test.h"

namespace test {
	class TestClearColor : public Test {
	
	public:
		TestClearColor(GLFWwindow* win);
		~TestClearColor();

		void onUpdate(float deltaTime) override;
		void onRender() override;
		void onImGuiRender() override;
	
	private:
		GLFWwindow* m_win;
		float m_clearColor[4];
	};
}