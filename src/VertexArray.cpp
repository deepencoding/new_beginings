#include "../headers/VertexArray.h"
#include "../headers/util.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_ID));
	GLCall(glBindVertexArray(m_ID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_ID));
}

void VertexArray::addVertexBuffer
(const VertexBuffer& vb, const VertexBufferLayout& layout) const
{
	bind();
	vb.bind();

	const auto& elements = layout.getElements();
	unsigned int offset = 0;
	
	for (unsigned int i = 0; i < elements.size(); ++i) {
		const auto& ele = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, ele.count, ele.type, ele.normalized, layout.getStride(), (const void*)offset));

		offset += ele.count * VertexBufferElement::getTypeSize(ele.type);
	}
}

void VertexArray::addIndexBuffer(const IndexBuffer& ib) const
{
}

void VertexArray::bind() const
{
	GLCall(glBindVertexArray(m_ID));
}

void VertexArray::unbind() const
{
	GLCall(glBindVertexArray(0));
}
