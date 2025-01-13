#pragma once

#include <vector>
#include <string>
#include <functional>

#include "../../headers/util.h"

namespace test {
	class Test {
	public:
		Test() {}
		virtual ~Test() {}

		virtual void onUpdate(float deltaTime) {}
		virtual void onRender() {
			GLCall(glClearColor(0.f, 0.f, 0.f, 1.f));
			GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		}
		virtual void onImGuiRender() {}
	};

	class TestMenu : public Test {
	public:
		TestMenu(Test*& curTestPointer);

		void onImGuiRender() override;
		
		template<typename T>
		void registerTest(const std::string& name) {
			std::cout << "Registering test: " << name << std::endl;

			m_tests.push_back(std::make_pair(name, []() { return new T(); }));
		}

	private:
		Test*& m_currentTest;
		std::vector<std::pair<std::string, std::function<Test* ()>>> m_tests;
	};
}