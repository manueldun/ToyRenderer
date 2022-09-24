#include "Utils.h"
#include<fstream>
#include<sstream>
std::string Utils::loadTextFile(const std::string& path)
{
	std::ifstream fileStream(path.c_str());
	if (!fileStream.good())
	{
		throw std::runtime_error("error opening file meshfile\n" + path);
	}
	std::stringstream buffer;
	buffer << fileStream.rdbuf();
	return buffer.str();
}



const std::vector<std::string_view> Utils::separateData(const std::string_view& str, const char* separator) {

	std::vector<std::string_view> separatedData;
	size_t floatBegin = 0;
	size_t floatEnd = 0;
	std::string_view subString;
	std::string_view leftData = str;
	do
	{

		floatEnd = leftData.find(separator);

		subString= leftData.substr(floatBegin, floatEnd);
		leftData = leftData.substr(floatEnd+1);
		separatedData.push_back(subString);
	} while (floatEnd !=std::string_view::npos);
	return separatedData;
}

const std::vector<float> Utils::convertToFloat(const std::vector<std::string_view>& str)
{
	std::vector<float> floatVector;
	for (const auto& floatData : str)
	{
		floatVector.push_back(std::stof(std::string(floatData)));
	}
	return floatVector;
}

const std::vector<unsigned int> Utils::convertToInt(const std::vector<std::string_view>& str)
{
	std::vector<unsigned int> uintVector;
	for (const auto& floatData : str)
	{
		uintVector.push_back(std::stoi(std::string(floatData)));
	}
	return uintVector;
}

Utils::InterleavedData Utils::interlieveData(
	const std::vector<float>& rawPositions,
	const std::vector<float>& rawTexCoords,
	const std::vector<float>& rawNormals,
	const std::vector<std::vector<unsigned int>>& rawIndices,
	std::unordered_map<Utils::OBJIndex, size_t, Utils::hash_fn>& mapIndices,
	unsigned int& newDataCount)
{
	std::vector<float> vertexData;
	std::vector<unsigned int> indexData;
	for (size_t i = 0; i < rawIndices.size(); i += 3)
	{

		std::array<float, 3> position1 = { 
			rawPositions.at(( rawIndices.at(i).at(std::size_t(0)) - std::size_t(1)) * std::size_t(3)),
			rawPositions.at(((rawIndices.at(i).at(std::size_t(0)) - std::size_t(1)) * std::size_t(3)) + std::size_t(1)),
			rawPositions.at(((rawIndices.at(i).at(std::size_t(0)) - std::size_t(1)) * std::size_t(3)) + std::size_t(2))

		};

		std::array<float, 2> texCoord1 = {
			rawTexCoords.at((rawIndices.at(i).at(std::size_t(1)) - std::size_t(1)) * std::size_t(2)),
			rawTexCoords.at((rawIndices.at(i).at(std::size_t(1)) - std::size_t(1)) * std::size_t(2) + std::size_t(1))
		};

		std::array<float, 3> position2 = {
			rawPositions.at((rawIndices.at(i + std::size_t(1)).at(std::size_t(std::size_t(0))) - std::size_t(1)) * std::size_t(3)),
			rawPositions.at((rawIndices.at(i + std::size_t(1)).at(std::size_t(std::size_t(0))) - std::size_t(1)) * std::size_t(3) + std::size_t(1)),
			rawPositions.at((rawIndices.at(i + std::size_t(1)).at(std::size_t(std::size_t(0))) - std::size_t(1)) * std::size_t(3) + std::size_t(2))

		};

		std::array<float, 2> texCoord2 = {
			rawTexCoords.at((rawIndices.at(i + std::size_t(1)).at(std::size_t(1)) - std::size_t(1)) * std::size_t(2)),
			rawTexCoords.at((rawIndices.at(i + std::size_t(1)).at(std::size_t(1)) - std::size_t(1)) * std::size_t(2) + std::size_t(1))
		};
		std::array<float, 3> position3 = {
			rawPositions.at((rawIndices.at(i + std::size_t(2)).at(std::size_t(0)) - std::size_t(1)) * std::size_t(3)),
			rawPositions.at((rawIndices.at(i + std::size_t(2)).at(std::size_t(0)) - std::size_t(1)) * std::size_t(3) + std::size_t(1)),
			rawPositions.at((rawIndices.at(i + std::size_t(2)).at(std::size_t(0)) - std::size_t(1)) * std::size_t(3) + std::size_t(2))

		};

		std::array<float, 2> texCoord3 = {
			rawTexCoords.at((rawIndices.at(i + std::size_t(2)).at(std::size_t(1)) - std::size_t(1)) * std::size_t(2)),
			rawTexCoords.at((rawIndices.at(i + std::size_t(2)).at(std::size_t(1)) - std::size_t(1)) * std::size_t(2) + std::size_t(1))
		};
		//glm::vec3 position1 = glm::vec3(
		//	rawPositions.at(( i->at(0) - 1) * 3),
		//	rawPositions.at(((i->at(0) - 1) * 3) + 1),
		//	rawPositions.at(((i->at(0) - 1) * 3) + 2));
		//glm::vec3 position2 = glm::vec3(
		//	rawPositions.at(((i+1)->at(0) - 1) * 3),
		//	rawPositions.at(((i+1)->at(0) - 1) * 3 + 1),
		//	rawPositions.at(((i+1)->at(0) - 1) * 3 + 2));
		//glm::vec3 position3 = glm::vec3(
		//	rawPositions.at(((i+2)->at(0) - 1) * 3),
		//	rawPositions.at(((i+2)->at(0) - 1) * 3 + 1),
		//	rawPositions.at(((i+2)->at(0) - 1) * 3 + 2));

		//glm::vec2 texCoord1 = glm::vec2(
		//	rawTexCoords.at(((i)->at(1) - 1) * 2),
		//	rawTexCoords.at(((i)->at(1) - 1) * 2 + 1)
		//);
		//glm::vec2 texCoord2 = glm::vec2(
		//	rawTexCoords.at(((i + 1)->at(1) - 1) * 2),
		//	rawTexCoords.at(((i + 1)->at(1) - 1) * 2 + 1)
		//);
		//glm::vec2 texCoord3 = glm::vec2(
		//	rawTexCoords.at(((i + 2)->at(1) - 1) * 2),
		//	rawTexCoords.at(((i + 2)->at(1) - 1) * 2 + 1)
		//);
		std::array<float, 3> tangent1 = calculateTangents(
			position1,
			position2,
			position3,
			texCoord1,
			texCoord2,
			texCoord3);
		std::array<float, 3> tangent2 = calculateTangents(
			position2,
			position1,
			position3,
			texCoord2,
			texCoord1,
			texCoord3);
		std::array<float, 3> tangent3 = calculateTangents(
			position3,
			position1,
			position2,
			texCoord3,
			texCoord1,
			texCoord2);
		std::array<float, 3> tangents[3] = { tangent1, tangent2, tangent3 };
		unsigned int tangentIndex = 0;
		for (auto j = i; j < i + 3; ++j)
		{

			Utils::OBJIndex index = Utils::OBJIndex(rawIndices.at(j).at(0), rawIndices.at(j).at(1), rawIndices.at(j).at(2));
			if (!mapIndices.contains(index))
			{
				size_t outputIndex = newDataCount;
				indexData.push_back(outputIndex);
				mapIndices[index] = outputIndex;
				newDataCount++;

				vertexData.push_back(rawPositions.at((rawIndices.at(j).at(std::size_t(0)) - std::size_t(1)) * std::size_t(3)));
				vertexData.push_back(rawPositions.at((rawIndices.at(j).at(std::size_t(0)) - std::size_t(1)) * std::size_t(3) + std::size_t(1)));
				vertexData.push_back(rawPositions.at((rawIndices.at(j).at(std::size_t(0)) - std::size_t(1)) * std::size_t(3) + std::size_t(2)));

				vertexData.push_back(rawNormals.at((rawIndices.at(j).at(std::size_t(2)) - std::size_t(1)) * std::size_t(3)));
				vertexData.push_back(rawNormals.at((rawIndices.at(j).at(std::size_t(2)) - std::size_t(1)) * std::size_t(3) + std::size_t(1)));
				vertexData.push_back(rawNormals.at((rawIndices.at(j).at(std::size_t(2)) - std::size_t(1)) * std::size_t(3) + std::size_t(2)));

				vertexData.push_back(rawTexCoords.at((rawIndices.at(j).at(std::size_t(1)) - std::size_t(1)) * std::size_t(2)));
				vertexData.push_back(rawTexCoords.at((rawIndices.at(j).at(std::size_t(1)) - std::size_t(1)) * std::size_t(2) + std::size_t(1)));


				vertexData.push_back(tangents[tangentIndex][std::size_t(0)]);
				vertexData.push_back(tangents[tangentIndex][std::size_t(1)]);
				vertexData.push_back(tangents[tangentIndex][std::size_t(2)]);

				tangentIndex++;
			}
			else
			{
				indexData.push_back(mapIndices[index]);
			}
		}
	}
	InterleavedData interleavedData;
	interleavedData.indexData = indexData;
	interleavedData.vertexData = vertexData;
	return interleavedData;
}

std::array<float, 3> Utils::calculateTangents(
	const std::array<float, 3>& position1,
	const std::array<float, 3>& position2,
	const std::array<float, 3>& position3,
	const std::array<float, 2>& uv1,
	const std::array<float, 2>& uv2,
	const std::array<float, 2>& uv3)
{


	
	float edge1[3] = {
		position2[0] - position1[0],
		position2[1] - position1[1],
		position2[2] - position1[2]
	};
	float edge2[3] = {
		position3[0] - position1[0],
		position3[1] - position1[1],
		position3[2] - position1[2]
	};



	float deltaUV1[2] = {
		uv2[0] - uv1[0],
		uv2[1] - uv1[1]
	};
	float deltaUV2[2] = {
		uv3[0] - uv1[0],
		uv3[1] - uv1[1]
	};

	
	std::array<float, 3> tangent;;
	float f = 1.0f / ((deltaUV1[0] * deltaUV2[1])-(deltaUV2[0] * deltaUV1[1]));
	tangent[0] = f * (deltaUV2[1] * edge1[0] - deltaUV1[1] * edge2[0]);
	tangent[1] = f * (deltaUV2[1] * edge1[1] - deltaUV1[1] * edge2[1]);
	tangent[2] = f * (deltaUV2[1] * edge1[2] - deltaUV1[1] * edge2[2]);
	return tangent;
}
