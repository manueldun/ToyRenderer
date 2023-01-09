#pragma once
#include<memory>
#include"OGL4/Texture.h"
#include"OGL4/Shader.h"
#include"RawTexture.h"
class PBRMaterial {
public:
	PBRMaterial(
		const float redAlbedo,
		const float greenAlbedo,
		const float blueAlbedo,
		const float roughness,
		const float metalness,
		const RawTexture& albedoRawTexture,
		const RawTexture& normalRawTexture,
		const RawTexture& roughnessMetalnessRawTexture,
		const bool hasTangent);
	//GPU
	void loadToGPU();
	void setMaterialUniforms(std::shared_ptr<Shader> shader) const;
	RawTexture getAlbedoTexture()const;
	RawTexture getNormalTexture()const;
	RawTexture getRoughnessMetalnessTexture()const;
	std::vector<float> getAlbedoConstants()const;
private:
	bool m_hasTangent;
	//scalars
	float m_redAlbedo;
	float m_greenAlbedo;
	float m_blueAlbedo;
	float m_roughness;
	float m_metalness;


	//textures
	RawTexture m_albedoRawTexture;
	RawTexture m_normalRawTexture;
	RawTexture m_roughnessMetalnessRawTexture;

};