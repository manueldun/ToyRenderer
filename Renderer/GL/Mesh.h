#pragma once
#include<vector>
#include<memory>
#include"VertexArray.h"
#include"SubMesh.h"
class Mesh {
public:
	Mesh(
		std::vector<SubMesh> subMeshes,
		std::vector<std::vector<float>> vertexBufferArray);
	const std::vector<SubMesh>& getSubMeshes()const;
	void loadToGPU();
	void bind() const;
	bool isInGPU() const;
private:
	std::vector<SubMesh> m_subMeshes;
	const std::vector<std::vector<float>> m_vertexBufferArray;

	bool inGPU = false;
	//GPU
	std::shared_ptr<VertexArray> m_vertexArray;

};


