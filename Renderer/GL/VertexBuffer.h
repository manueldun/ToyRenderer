#pragma once
#include<vector>

#include"Layout.h"

class VertexBuffer {
public:
	VertexBuffer(
		const unsigned int location,
		const unsigned int elementCount);
	VertexBuffer(const std::vector<float>& buffer, unsigned int location, unsigned int numOfAttribs);
	VertexBuffer(const void* data, const size_t size, Utils::Layout layout);//interlieved
	~VertexBuffer();
	void bind() const;
	void loadToGPU();
private:
	bool inGPU = false;
	std::vector<float> m_buffer;
	const int m_location;
	const int m_numOfAttribs;
	unsigned int m_VBO;
};