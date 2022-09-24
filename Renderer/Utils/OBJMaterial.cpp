#include"OBJMaterial.h"

Utils::OBJMaterial::OBJMaterial(
	std::string materialName,
	float diffuseRed,
	float diffuseGreen,
	float diffuseBlue,
	float specularRed,
	float specularGreen,
	float specularBlue,
	float shininess,
	std::string diffuseMapPath,
	std::string bumpMapPath)
	:m_materialName(materialName),
	m_diffuseRed(diffuseRed),
	m_diffuseGreen(diffuseGreen),
	m_diffuseBlue(diffuseBlue),
	m_specularRed(specularRed),
	m_specularGreen(specularGreen),
	m_specularBlue(specularBlue),
	m_shininess(shininess),
	m_diffuseMapPath(diffuseMapPath),
	m_bumpMapPath(bumpMapPath)
{

}
