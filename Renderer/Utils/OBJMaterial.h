#pragma once
#include<string>
namespace Utils {
	class OBJMaterial {
	public:
		OBJMaterial() = delete;
			OBJMaterial(
				std::string materialName,
				float diffuseRed,
				float diffuseGreen,
				float diffuseBlue,
				float m_speculaRed,
				float m_speculaGreen,
				float m_speculaBlue,
				float shininess,
				std::string bumpMapPath,
				std::string diffuseMapPath);
		std::string m_materialName;
		float m_diffuseRed;
		float m_diffuseGreen;
		float m_diffuseBlue;
		float m_specularRed;
		float m_specularGreen;
		float m_specularBlue;
		float m_shininess;
		std::string m_diffuseMapPath;
		std::string m_bumpMapPath;
	};
}