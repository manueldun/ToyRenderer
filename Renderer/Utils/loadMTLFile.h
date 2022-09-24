#pragma once
#include<vector>
#include<string>
#include<unordered_map>
#include<memory>
#include"OBJMaterial.h"
#include<string_view>

namespace Utils {

	std::vector<std::shared_ptr<OBJMaterial>> loadMTLFile(const std::string_view& path,std::unordered_map<std::string,size_t> &nameToMaterialMap);
}