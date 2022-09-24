#include"Mesh.h"
#include <string>

Mesh::Mesh(const std::vector<SubMesh>& subMeshes, const std::vector<float>& vertexBufferArray)
	:m_subMeshes(subMeshes),
	m_vertexArray(std::unique_ptr<VertexArray>(new VertexArray))
{	

	m_vertexArray->bind();
	m_vertexBuffer =
		std::make_shared<VertexBuffer>(
			vertexBufferArray.data(),
			vertexBufferArray.size(),
			Utils::tangentLayout);
	

}

Mesh::Mesh(const std::vector<SubMesh>& subMesh, const std::shared_ptr<VertexArray>& vertexArray)
	:m_subMeshes(subMesh),
	m_vertexArray(vertexArray)
{

}

std::shared_ptr<VertexArray> Mesh::getVertexArray()const
{
	return m_vertexArray;
}

std::vector<SubMesh> Mesh::getSubMeshes() const
{
	return m_subMeshes;
}
