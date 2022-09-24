#include"VertexBuffer.h"
#include<glad/glad.h>

VertexBuffer::VertexBuffer(
	const unsigned int location,
	const unsigned int elementCount)
{
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glVertexAttribPointer(location, elementCount, GL_FLOAT, GL_FALSE, 0, 0);
}

VertexBuffer::VertexBuffer(const void* data, size_t size, unsigned int location, unsigned int numOfAttrib)
{
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glVertexAttribPointer(location, numOfAttrib, GL_FLOAT, GL_FALSE, 0, 0);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), data, GL_STATIC_DRAW);
}

VertexBuffer::VertexBuffer(const void* data, const size_t size, Utils::Layout layout)
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
	glDeleteBuffers(1, &m_VBO);
}

void VertexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
}
