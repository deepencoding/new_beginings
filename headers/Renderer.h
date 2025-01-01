#pragma once

#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"

class Renderer
{
public:
	void clear() const;
	void draw(const VertexArray& va, const Shader& shader) const;
	void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};

