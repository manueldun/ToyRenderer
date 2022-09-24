#pragma once
#include<string>
#include<vector>
#include<array>
#include<unordered_map>
#include"OBJData.h"
#include<glm/glm.hpp>
#include<string_view>
namespace Utils {

	struct OBJIndex
	{
		unsigned int x;
		unsigned int y;
		unsigned int z;

		OBJIndex(unsigned int x, unsigned int y, unsigned int z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}

		bool operator==(const OBJIndex& p) const {
			return x == p.x && y == p.y && z == p.z;
		}
	};

	struct hash_fn
	{
		std::size_t operator() (const OBJIndex& node) const
		{
			std::size_t h1 = std::hash<unsigned int>()(node.x);
			std::size_t h2 = std::hash<unsigned int>()(node.y);
			std::size_t h3 = std::hash<unsigned int>()(node.z);

			return h1 ^ h2 ^ h3;
		}
	};

	struct InterleavedData {
		std::vector<unsigned int> indexData;
		std::vector<float> vertexData;

	};
	std::string loadTextFile(const std::string& file);
	const std::vector<std::string_view> separateData(const std::string_view& str, const char* separator);
	const std::vector<float> convertToFloat(const std::vector<std::string_view>& str);
	const std::vector<unsigned int> convertToInt(const std::vector<std::string_view>& str);

	Utils::InterleavedData interlieveData(
		const std::vector<float>& rawPositions,
		const std::vector<float>& rawTexCoords,
		const std::vector<float>& rawNormals,
		const std::vector<std::vector<unsigned int>>& rawIndices,
		std::unordered_map<Utils::OBJIndex, size_t, Utils::hash_fn>& mapIndices,
		unsigned int& newDataCount);



	std::array<float, 3> calculateTangents(
		const std::array<float,3>& position1,
		const std::array<float,3>& position2,
		const std::array<float,3>& position3,
		const std::array<float,2>& uv1,
		const std::array<float,2>& uv2,
		const std::array<float,2>& uv3);

	
};
