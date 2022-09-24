#pragma once

#include<vector>
#include"PBRMaterial.h"
#include"Mesh.h"
#include"Utils.h"
#include"tiny_gltf.h"
#include"OGL4/Shader.h"
std::vector<Mesh> convertToMeshes(
	const std::vector<Utils::OBJData>& objData,
	const std::vector<std::shared_ptr<Shader>>& shaders);
std::vector<Mesh> convertToMeshes(
	const tinygltf::Model& model,
	const std::shared_ptr<Shader>& shader);
std::vector<float> convertDataToFloatBuffer(
	const std::vector<unsigned char>& data,
	size_t offset,
	size_t numOfElements);
std::vector<unsigned short> convertDataToUIntBuffer(
	const std::vector<unsigned char>& data,
	size_t offset,
	size_t numOfElements);



std::vector<Mesh> separateIndicesFromVertexData(const tinygltf::Model& model, std::shared_ptr<Shader> shader);