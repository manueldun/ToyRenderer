#pragma once
#include<vector>
#include<memory>
#include"OGL4/IndexBuffer.h"
#include"Material.h"
#include"PBRMaterial.h"
class SubMesh {
public:
	SubMesh(const std::shared_ptr<IndexBuffer> indexBuffer, const std::shared_ptr<Material> material);
	std::shared_ptr<IndexBuffer> getIndexBufferID()const;
	std::shared_ptr<Material> getMaterial()const;
private:
	std::shared_ptr<IndexBuffer> m_indexBufferID;
	std::shared_ptr<Material> m_material;
};
