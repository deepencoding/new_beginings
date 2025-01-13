#include <iostream>

#include "Test.h"
#include "../vendor/imgui/imgui.h"

namespace test {
	TestMenu::TestMenu(Test*& curTestPointer)
		: m_currentTest (curTestPointer) 
	{
	}

	void TestMenu::onImGuiRender()
	{
		for (const auto& test : m_tests) {
			if (ImGui::Button(test.first.c_str())) {
				m_currentTest = test.second();
			}
		}
	}
}
