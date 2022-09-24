#include"OBJData.h"
Utils::OBJData::OBJData(
	const std::string& name,
	const std::vector<std::string>& groupsName,
	const std::vector<SubOBJData>& subOBJDatas,
	const std::vector<float>& vertexBuffer) :
	m_name(name),
	m_groupsName(groupsName),
	m_subOBJDatas(subOBJDatas),
	m_vertexBuffer(vertexBuffer)
{
}

const std::vector<Utils::SubOBJData> Utils::OBJData::getSubOBJDatas() const
{
	return m_subOBJDatas;
}

const std::vector<float> Utils::OBJData::getVertexBuffer() const
{
	return m_vertexBuffer;
}

