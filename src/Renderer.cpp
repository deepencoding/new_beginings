
#include "../headers/util.h"
#include "../headers/Renderer.h"

void Renderer::clear(std::vector<float> cols) const
{
	if (cols.size() != 4) {
		std::cout << "ERROR::RENDERER::INVALID CLEAR COLOR\n";
		return;
	}
	GLCall(glClearColor(cols[0], cols[1], cols[2], cols[3]));
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