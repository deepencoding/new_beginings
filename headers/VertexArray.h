#pragma once

#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void addVertexBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) const;
	void addIndexBuffer(const IndexBuffer& ib) const;

	void bind() const;
	void unbind() const;

private:
	unsigned int m_ID;
};

