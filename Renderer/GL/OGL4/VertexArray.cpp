#include"VertexArray.h"
#include<glad/glad.h>
VertexArray::VertexArray()
	:m_vertexBuffers({ nullptr })
{
	glGenVertexArrays(1, &m_va);
	bind();
}

VertexArray::VertexArray(const std::vector<std::shared_ptr<VertexBuffer>>& vertexBuffers)
	:m_vertexBuffers(vertexBuffers)
{
	glGenVertexArrays(1, &m_va);
	bind();
	for (const auto& vertexBuffer : vertexBuffers)
	{
		vertexBuffer->loadToGPU();
	}
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_va);
}

void VertexArray::bind() const
{
	glBindVertexArray(m_va);
}

void VertexArray::setVertexBuffers(const std::vector<std::shared_ptr<VertexBuffer>>& vertexBuffers)
{
	m_vertexBuffers = vertexBuffers;
}

void VertexArray::unbind()
{
	glBindVertexArray(0);
}
