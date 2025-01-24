#pragma once

#include <vector>

#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"

class Renderer
{
public:
	void clear(std::vector<float> cols = { 0.2f, 0.3f, 0.3f, 1.0f }) const;
	void draw(const VertexArray& va, const Shader& shader) const;
	void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};

