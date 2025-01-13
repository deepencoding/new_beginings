#pragma once

#include "Test.h"

namespace test {
	class TestTexture2D : public Test {
	
	public:
		TestTexture2D();
		~TestTexture2D();

		void onUpdate(float deltaTime) override;
		void onRender() override;
		void onImGuiRender() override;
	};
}