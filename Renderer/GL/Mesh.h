#pragma once
#include<vector>
#include<string>
#include<memory>
#include"Layout.h"

#include"OGL4/VertexArray.h"
#include"OGL4/VertexBuffer.h"
#include"SubMesh.h"

class Mesh {
public:

	Mesh(
		const std::vector<SubMesh>& subMesh,
		const std::vector<float>& vertexBufferArray);
	Mesh(
		const std::vector<SubMesh>& subMesh,
		const std::shared_ptr<VertexArray>& vertexArray);
	std::shared_ptr<VertexArray> getVertexArray()const;
	std::vector<SubMesh> getSubMeshes()const;
private:

	std::shared_ptr<VertexArray> m_vertexArray;
	std::shared_ptr<VertexBuffer> m_vertexBuffer;
	std::vector<SubMesh> m_subMeshes;
};


