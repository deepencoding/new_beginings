
#include "../headers/util.h"
#include "../headers/Renderer.h"

void Renderer::clear() const
{
	GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::draw(const VertexArray& va, const Shader& shader) const
{
	shader.bind();
	va.bind();
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
}

void Renderer::draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	shader.bind();
	va.bind();
	GLCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, 0));
}
