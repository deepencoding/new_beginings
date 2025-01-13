#include <iostream>

#include "Test.h"
#include "../vendor/imgui/imgui.h"

test::TestMenu::TestMenu(Test*& curTestPointer)
	: m_currentTest (curTestPointer) 
{
}

void test::TestMenu::onImGuiRender()
{
	for (const auto& test : m_tests) {
		if (ImGui::Button(test.first.c_str())) {
			m_currentTest = test.second();
		}
	}
}