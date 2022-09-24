#pragma once
#include<vector>

#include"Layout.h"

class VertexBuffer {
public:
	VertexBuffer(
		const unsigned int location,
		const unsigned int elementCount);
	VertexBuffer(const void* data, size_t size, unsigned int location, unsigned int numOfAttrib);
	VertexBuffer(const void* data, const size_t size, Utils::Layout layout);
	~VertexBuffer();
	void bind() const;
private:
	unsigned int m_VBO;
};