#include "VertexBuffer.h"
#include"VertexBuffer.h"
#include<glad/glad.h>

VertexBuffer::VertexBuffer(
	const unsigned int location,
	const unsigned int elementCount)
	:m_location(location),
	m_numOfAttribs(elementCount)
{
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glVertexAttribPointer(location, elementCount, GL_FLOAT, GL_FALSE, 0, 0);
}

VertexBuffer::VertexBuffer(const std::vector<float>& buffer, unsigned int location, unsigned int numOfAttribs)
	:m_buffer(buffer),
	m_location(location),
	m_numOfAttribs(numOfAttribs)
{
	m_VBO = 0;
}
VertexBuffer::VertexBuffer(const void* data, const size_t size, Utils::Layout layout)
	:m_location(0),
	m_numOfAttribs(0)
{
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	unsigned int stride = 0;
	for (const auto& attributeLayout : layout.m_attributeLayouts)
	{
		stride += attributeLayout.m_elementCount;
	}

	for (const auto& attributeLayout : layout.m_attributeLayouts)
	{
		glVertexAttribPointer(
			attributeLayout.m_location,
			attributeLayout.m_elementCount,
			GL_FLOAT,
			GL_FALSE,
			stride * sizeof(float),
			(const void*)(attributeLayout.m_startOffset * sizeof(float)));
	}

	glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), data, GL_STATIC_DRAW);
}



VertexBuffer::~VertexBuffer()
{
	if (inGPU)
	{
		glDeleteBuffers(1, &m_VBO);
	}
}

void VertexBuffer::bind() const
{
	if (inGPU)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	}
}

void VertexBuffer::loadToGPU()
{
	inGPU = true;

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glVertexAttribPointer(
		m_location,
		m_numOfAttribs,
		GL_FLOAT,
		GL_FALSE,
		0,
		0);

	glBufferData(GL_ARRAY_BUFFER, m_buffer.size() * sizeof(float), m_buffer.data(), GL_STATIC_DRAW);

}
