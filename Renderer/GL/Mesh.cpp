#include"Mesh.h"

Mesh::Mesh(
	std::vector<SubMesh> subMeshes,
	std::vector<std::vector<float>> vertexBufferArray)
	:m_subMeshes(subMeshes),
	m_vertexBufferArray(vertexBufferArray)
{	

}


std::vector<SubMesh> Mesh::getSubMeshes() const
{
	return m_subMeshes;
}

void Mesh::loadToGPU()
{	
	inGPU = true;
	std::vector<std::shared_ptr< VertexBuffer>> vertexBufferArray =
	{
		std::make_shared<VertexBuffer>(m_vertexBufferArray.at(0),0,3),
		std::make_shared<VertexBuffer>(m_vertexBufferArray.at(1),1,2),
		std::make_shared<VertexBuffer>(m_vertexBufferArray.at(2),2,3),
		std::make_shared<VertexBuffer>(m_vertexBufferArray.at(3),3,4)
	};

	m_vertexArray = std::make_shared<VertexArray>(vertexBufferArray);
	
	for (auto& subMesh : m_subMeshes)
	{
		subMesh.loadToGPU();
	}
}
void Mesh::bind() const
{
	if (inGPU)
	{
		m_vertexArray->bind();
		
		
	}
}
