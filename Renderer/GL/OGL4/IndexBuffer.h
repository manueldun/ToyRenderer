#pragma once

#include <cstddef>
class IndexBuffer {
public:
	IndexBuffer(const void* data, const std::size_t count);
	IndexBuffer(
		const void* data, 
		const std::size_t count,
		const std::size_t bytesPerElement
	);
	IndexBuffer();
	~IndexBuffer();
	void bind() const;
	unsigned int getCount()const;
private:
	unsigned int m_VBO;
	std::size_t m_count;

};
