#pragma once
#include<vector>
#include<memory>
#include"OBJMaterial.h"
namespace Utils {
	class SubOBJData {
	public:
		SubOBJData(const std::vector<unsigned int>& indexBuffer,const std::shared_ptr<OBJMaterial>& objMaterial);
		std::vector<unsigned int> getIndexBuffer()const;
		std::shared_ptr<OBJMaterial> getOBJMaterial()const;
	private:
		const std::vector<unsigned int> m_indexBuffer;
		const std::shared_ptr<OBJMaterial> m_objMaterial;
	};
}