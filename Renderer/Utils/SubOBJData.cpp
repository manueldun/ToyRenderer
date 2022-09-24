#include"SubOBJData.h"

Utils::SubOBJData::SubOBJData(const std::vector<unsigned int>& indexBuffer, const std::shared_ptr<OBJMaterial>& objMaterial)
	:m_indexBuffer(indexBuffer),
	m_objMaterial(objMaterial)
{
}

std::vector<unsigned int> Utils::SubOBJData::getIndexBuffer() const
{
	return m_indexBuffer;
}

std::shared_ptr<Utils::OBJMaterial> Utils::SubOBJData::getOBJMaterial() const
{
	return m_objMaterial;
}
