#include"IndexBuffer.h"
#include<glad/glad.h>

IndexBuffer::IndexBuffer(const void* data, const size_t count) :m_count(count)
{
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

IndexBuffer::IndexBuffer(
	const void* data, 
	const size_t count, 
	const size_t bytesPerElement=sizeof(unsigned int))
	:m_count(count)
{
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
		static_cast<unsigned long long>(count) * static_cast<unsigned long long>(bytesPerElement), data, GL_STATIC_DRAW);
}

IndexBuffer::IndexBuffer() :m_count(0)
{
	m_VBO = 0;
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_VBO);
}

void IndexBuffer::bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO);
}

unsigned int IndexBuffer::getCount() const
{
	return m_count;
}
