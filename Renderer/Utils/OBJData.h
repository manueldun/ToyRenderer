#pragma once
#include<vector>
#include<string>
#include"SubOBJData.h"
#include"Layout.h"
namespace Utils {

	class OBJData {
	public:
		OBJData(
			const std::string& name,
			const std::vector<std::string>& groupsName,
			const std::vector<SubOBJData>& subOBJData,
			const std::vector<float>& vertexBuffer);

		const std::vector<SubOBJData> getSubOBJDatas() const;

		const std::vector<float> getVertexBuffer() const;

	private:
		std::string m_name;
		std::vector<std::string> m_groupsName;
		std::vector<SubOBJData> m_subOBJDatas;
		std::vector<float> m_vertexBuffer;

	};
}