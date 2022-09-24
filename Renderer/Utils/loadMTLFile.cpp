#include"loadMTLFile.h"
#include"loadOBJFile.h"
#include<fstream>
#include"Utils.h"

std::vector <std::shared_ptr<Utils::OBJMaterial>> Utils::loadMTLFile(
	const std::string_view& path,
	std::unordered_map<std::string, size_t>& nameToMaterialMap)
{
	size_t lastSlashOfPath = path.find_last_of("/");
	const std::string currentDirectory = std::string(path.substr(0, lastSlashOfPath));
	std::ifstream file(std::string(path).c_str());

	if (!file.good())
	{
		throw std::runtime_error("error opening MTL file");
	}

	std::vector<std::shared_ptr<OBJMaterial>> objMaterials;
	std::string currentLabelName;
	float currentDiffuseRed = 1.0;
	float currentDiffuseGreen = 0.0;
	float currentDiffuseBlue = 1.0;
	float currentSpecularRed = 0.0;
	float currentSpecularGreen = 0.0;
	float currentSpecularBlue = 0.0;
	float currentSpecularity = 0.0;
	std::string currentDiffuseMapPath = "";
	std::string currentDispMapPath = "";
	if (file.is_open())
	{

		std::string line;
		while (getline(file, line))
		{

			size_t labelLength = line.find(" ");
			std::string label = line.substr(0, labelLength);
			if (label.compare("newmtl") == 0)
			{
				if (currentLabelName.length() != 0)
				{
					nameToMaterialMap[currentLabelName] = objMaterials.size();
					objMaterials.push_back(std::make_shared<OBJMaterial>(
						currentLabelName,
						currentDiffuseRed,
						currentDiffuseGreen,
						currentDiffuseBlue,
						currentSpecularRed,
						currentSpecularGreen,
						currentSpecularBlue,
						currentSpecularity,
						currentDiffuseMapPath,
						currentDispMapPath));
					currentDiffuseRed = 1.0;
					currentDiffuseGreen = 0.0;
					currentDiffuseBlue = 1.0;
					currentSpecularRed = 0.0;
					currentSpecularGreen = 0.0;
					currentSpecularBlue = 0.0;
					currentSpecularity = 0.0;
					currentDiffuseMapPath = "";
					currentDispMapPath = "";

				}
				line.erase(0, labelLength + 1);
				currentLabelName = line;
			}
			else if (label.compare("Ns") == 0)
			{
				currentSpecularity = std::stof(line.erase(0, labelLength + 1));
			}
			else if (label.compare("Kd") == 0)
			{
				line.erase(0, labelLength + 1);
				std::vector<float> currentDiffuseColor = convertToFloat(separateData(line, " "));
				currentDiffuseRed = currentDiffuseColor.at(0);
				currentDiffuseGreen = currentDiffuseColor.at(1);
				currentDiffuseBlue = currentDiffuseColor.at(2);

			}
			else if (label.compare("Ks") == 0)
			{
				line.erase(0, labelLength + 1);
				std::vector<float> currentSpecularColor = convertToFloat(separateData(line, " "));
				currentSpecularRed = currentSpecularColor.at(0);
				currentSpecularGreen = currentSpecularColor.at(1);
				currentSpecularBlue = currentSpecularColor.at(2);
			}
			else if (label.compare("Ns") == 0) {

				line.erase(0, labelLength + 1);
				currentSpecularity = std::stof(line);
			}
			else if (label.compare("map_Kd") == 0)
			{
				line.erase(0, labelLength + 1);
				currentDiffuseMapPath = currentDirectory + "/" + line;
			}
			else if (label.compare("map_Bump") == 0)
			{
				line.erase(0, labelLength + 1);
				currentDispMapPath = currentDirectory + "/" + line;
			}

		}
	}
	nameToMaterialMap[currentLabelName] = objMaterials.size();
	objMaterials.push_back(std::make_shared<OBJMaterial>(
		currentLabelName,
		currentDiffuseRed,
		currentDiffuseGreen,
		currentDiffuseBlue,
		currentSpecularRed,
		currentSpecularGreen,
		currentSpecularBlue,
		currentSpecularity,
		currentDiffuseMapPath,
		currentDispMapPath));
	return objMaterials;
}