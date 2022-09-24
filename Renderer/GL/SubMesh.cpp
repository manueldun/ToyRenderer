#include"SubMesh.h"



SubMesh::SubMesh(const std::shared_ptr<IndexBuffer> indexBuffer, const std::shared_ptr<Material> material)
	: m_indexBufferID(indexBuffer), m_material(material)
{
}

std::shared_ptr<IndexBuffer> SubMesh::getIndexBufferID() const
{
	return m_indexBufferID;
}

std::shared_ptr<Material> SubMesh::getMaterial() const
{
	return m_material;
}

