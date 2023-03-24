#pragma once
#include<vector>
#include<memory>
#include"OGL4/IndexBuffer.h"
#include"PBRMaterial.h"
class SubMesh {
public:
	SubMesh(const std::vector<unsigned short>& indexBuffer, const PBRMaterial& material);
	SubMesh(std::vector<unsigned short>&& indexBuffer, PBRMaterial&& material);
	const PBRMaterial& getMaterial()const;
	std::vector<unsigned short> getIndexBuffer();
	//GPU
	void loadToGPU();
	void bind()const;
	unsigned int getCount()const;
private:
	std::vector<unsigned short> m_indexBufferArray;
	PBRMaterial m_pbrMaterial;

	bool inGPU = false;
	//GPU
	std::shared_ptr<IndexBuffer> m_indexArray;
};
