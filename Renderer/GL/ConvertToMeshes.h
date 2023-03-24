#pragma once
#include"tiny_gltf.h"
#include<iostream>
#include<vector>
#include"Mesh.h"
#include"Utils.h"
std::vector<Mesh> convertToMeshes(
	tinygltf::Model& model);
std::vector<float> convertDataToFloatBuffer(
	const std::vector<unsigned char>& data,
	size_t offset,
	size_t numOfElements);
std::vector<unsigned short> convertDataToUIntBuffer(
	const std::vector<unsigned char>& data,
	size_t offset,
	size_t numOfElements);



std::vector<Mesh> separateIndicesFromVertexData(tinygltf::Model& model);