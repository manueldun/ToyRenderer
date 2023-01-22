#include"SubMesh.h"



SubMesh::SubMesh(std::vector<unsigned short> indexBuffer,PBRMaterial material)
	: m_indexBufferArray(indexBuffer), m_pbrMaterial(material)
{
}

const PBRMaterial& SubMesh::getMaterial() const
{
	return m_pbrMaterial;
}

std::vector<unsigned short> SubMesh::getIndexBuffer()
{
	return m_indexBufferArray;
}

void SubMesh::loadToGPU()
{
	inGPU = true;
	m_indexArray = std::make_shared<IndexBuffer>(m_indexBufferArray.data(), m_indexBufferArray.size());
	m_pbrMaterial.loadToGPU();

}
void SubMesh::bind() const
{
	m_indexArray->bind();
}

unsigned int SubMesh::getCount() const
{
	return m_indexArray->getCount();
}
