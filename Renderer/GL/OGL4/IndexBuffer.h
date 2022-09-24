#pragma once
class IndexBuffer {
public:
	IndexBuffer(const void* data, const size_t count);
	IndexBuffer(
		const void* data, 
		const size_t count,
		const size_t bytesPerElement
	);
	IndexBuffer();
	~IndexBuffer();
	void bind() const;
	unsigned int getCount()const;
private:
	unsigned int m_VBO;
	size_t m_count;

};
