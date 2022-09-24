#pragma once
#include"VertexBuffer.h"
#include<vector>
#include<memory>
class VertexArray {
public:
	VertexArray();
	VertexArray(const std::vector<std::shared_ptr<VertexBuffer>>& vertexBuffers);
	~VertexArray();
	void bind() const;
	void setVertexBuffers(const std::vector<std::shared_ptr<VertexBuffer>>& vertexBuffers);
	static void unbind();
private:
	unsigned int m_va;
	std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
};