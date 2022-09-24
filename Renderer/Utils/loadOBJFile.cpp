#include"loadOBJFile.h"
#include<unordered_map>
#include<fstream>
#include<memory>
#include"loadMTLFile.h"
#include"SubOBJData.h"
#include"Utils.h"
#include <sstream>
std::vector<Utils::OBJData> Utils::loadOBJFile(const std::string& path)
{
	unsigned int newDataCount = 0;
	std::string objStr = Utils::loadTextFile(path);
	std::istringstream f(objStr);

	size_t lastSlashOfPath = path.find_last_of("/");
	const std::string currentDirectory = path.substr(0, lastSlashOfPath);
	std::vector<float> rawPositions;
	std::vector<float> rawTexCoords;
	std::vector<float> rawNormals;
	std::vector<std::vector<unsigned int>> rawIndices;
	std::unordered_map<Utils::OBJIndex, size_t, Utils::hash_fn> mapIndices;

	std::vector<Utils::OBJData> outputData;


	std::vector<std::vector<unsigned int>> currentIndexBuffers;
	std::vector<std::shared_ptr<OBJMaterial>> currentMaterials;
	std::string_view currentMaterialName;
	std::vector<unsigned int> currentIndexBuffer;
	std::vector<float> currentVertexBuffer;
	std::vector<SubOBJData> currentSubOBJData;

	std::string currentName = "default";
	std::vector<std::string> currentGroups = { "default" };
	std::vector<std::shared_ptr<OBJMaterial>> loadedMaterials;
	std::unordered_map<std::string, size_t> nameToMaterialMap;

	std::string_view objStrView = objStr;
	std::size_t startOfLine = 0;
	std::size_t endOfLine = objStrView.find('\n');

	while (endOfLine != std::string_view::npos)
	{
		startOfLine = 0;
		endOfLine = objStrView.find('\n');
		std::string_view lineView = objStrView.substr(startOfLine, endOfLine + 1);
		objStrView = objStrView.substr(endOfLine + 1, std::string_view::npos);

		size_t labelLength = lineView.find(" ");
		std::string_view labelView = lineView.substr(0, labelLength);
		if (labelView.compare("o") == 0)
		{
			if (rawIndices.size() != 0)
			{
				Utils::InterleavedData interleavedData = interlieveData(
					rawPositions,
					rawTexCoords,
					rawNormals,
					rawIndices,
					mapIndices,
					newDataCount);

				rawPositions.clear();
				rawTexCoords.clear();
				rawNormals.clear();

				currentSubOBJData.push_back(
					SubOBJData(interleavedData.indexData, loadedMaterials[nameToMaterialMap[std::string(currentMaterialName)]]));
				outputData.push_back(OBJData(
					currentName,
					currentGroups,
					currentSubOBJData,
					interleavedData.vertexData));
				currentMaterials.clear();
				currentSubOBJData.clear();

			}


			currentName = lineView.substr(labelLength + 1, lineView.size() - labelLength - 2);


		}
		else if (labelView.compare("g") == 0) {


		}
		else if (labelView.compare("v") == 0)
		{
			size_t dataStart = lineView.find_first_not_of(" ", 1);
			std::string_view vertex_strv = lineView.substr(dataStart);

			std::vector<float> separatedVertexPosition = convertToFloat(separateData(vertex_strv, " "));

			for (const auto& position : separatedVertexPosition) {
				rawPositions.push_back(position);
			}
		}
		else if (labelView.compare("f") == 0)
		{
			size_t dataStart = lineView.find_first_not_of(" ", 1);
			std::string_view face_strv = lineView.substr(dataStart,lineView.size()-dataStart-1);

			const std::vector<std::string_view> faceindices = separateData(face_strv, " ");

			for (const auto& indicesStr : faceindices)
			{
				std::vector<unsigned int> vertexIndices = convertToInt(separateData(indicesStr, "/"));
				rawIndices.push_back(vertexIndices);
			}

		}
		else if (labelView.compare("vn") == 0)
		{
			size_t dataStart = lineView.find_first_not_of(" ", 2);
			std::string_view normal_strv = lineView.substr(dataStart);


			std::vector<float> separatedVertexNormals = convertToFloat(separateData(normal_strv, " "));

			for (const auto& normal : separatedVertexNormals)
			{
				rawNormals.push_back(normal);
			}
		}
		else if (labelView.compare("vt") == 0)
		{
			size_t dataStart = lineView.find_first_not_of(" ", 2);
			std::string_view coords_strv = lineView.substr(dataStart);


			std::vector<float> separatedTexCoord = convertToFloat(separateData(coords_strv, " "));

			for (const auto& texCoord : separatedTexCoord)
			{
				rawTexCoords.push_back(texCoord);
			}
		}
		else if (labelView.compare("mtllib") == 0)
		{
			size_t dataStart = lineView.find(" ");
			std::string_view materialFileName = lineView.substr(dataStart + 1, lineView.size() - 2 - dataStart);

			std::string mtlFilePath = currentDirectory + "/" + std::string(materialFileName);
			loadedMaterials = loadMTLFile(mtlFilePath, nameToMaterialMap);

		}
		else if (labelView.compare("usemtl") == 0)
		{
			size_t dataStart = lineView.find(" ");
			std::string_view materialName = lineView.substr(dataStart + 1, lineView.size() - 2 - dataStart);
			if (currentMaterialName.length() != 0)
			{
				Utils::InterleavedData interleavedData = interlieveData(
					rawPositions,
					rawTexCoords,
					rawNormals,
					rawIndices,
					mapIndices,
					newDataCount);


				currentVertexBuffer.reserve(currentVertexBuffer.size() + interleavedData.vertexData.size());
				currentVertexBuffer.insert(currentVertexBuffer.end(), interleavedData.vertexData.begin(), interleavedData.vertexData.end());
				currentIndexBuffers.push_back(interleavedData.indexData);
				currentMaterials.push_back(loadedMaterials[nameToMaterialMap[std::string(currentMaterialName)]]);
				currentSubOBJData.push_back(
					SubOBJData(interleavedData.indexData, loadedMaterials[nameToMaterialMap[std::string(currentMaterialName)]])
				);
			}

			currentMaterialName = materialName;

		}

	}
	Utils::InterleavedData interleavedData = interlieveData(
		rawPositions,
		rawTexCoords,
		rawNormals,
		rawIndices,
		mapIndices,
		newDataCount);


	currentVertexBuffer.reserve(currentVertexBuffer.size() + interleavedData.vertexData.size());
	currentVertexBuffer.insert(currentVertexBuffer.end(), interleavedData.vertexData.begin(), interleavedData.vertexData.end());
	currentIndexBuffers.push_back(interleavedData.indexData);

	currentMaterials.push_back(loadedMaterials[nameToMaterialMap[std::string(currentMaterialName)]]);

	currentSubOBJData.push_back(
		SubOBJData(interleavedData.indexData, loadedMaterials[nameToMaterialMap[std::string(currentMaterialName)]])
	);
	outputData.push_back(OBJData(
		currentName,
		currentGroups,
		currentSubOBJData,
		currentVertexBuffer));


	return outputData;
}