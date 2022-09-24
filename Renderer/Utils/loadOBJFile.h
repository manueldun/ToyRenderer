#pragma once
#include<vector>
#include<string>
#include"OBJData.h"
namespace Utils {

	std::vector<OBJData> loadOBJFile(const std::string& path);
}