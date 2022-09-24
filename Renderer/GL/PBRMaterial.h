#pragma once
#include<memory>
#include"OGL4/Texture.h"
#include"OGL4/Shader.h"
#include"Material.h"
class PBRMaterial:public Material {
public:
	PBRMaterial() = delete;
	PBRMaterial(
		const float redAlbedo,
		const float greenAlbedo,
		const float blueAlbedo,
		const float roughness,
		const float metallness,
		const std::shared_ptr<Texture> albedoTexture,
		const std::shared_ptr<Texture> normalTexture,
		const std::shared_ptr<Texture> roughnessMetalnessTexture,
		const std::shared_ptr<Shader> shader,
		const bool hasTangent);
	void bind();
	void setMaterialUniforms(std::shared_ptr<Shader> shader);
	std::shared_ptr<Shader> getShader();
	std::shared_ptr<Texture> getNormalTexture();
	std::shared_ptr<Texture> getAlbedoTexture();
	std::vector<float> getAlbedoConstants();
	const int getUniformLocation(const std::string& uniformName);
	std::shared_ptr<Shader> m_shader;

	const bool m_hasTangent;
	//scalars
	const float m_redAlbedo;
	const float m_greenAlbedo;
	const float m_blueAlbedo;
	const float m_roughness;
	const float m_metallness;
	

	//uniforms
	const int m_albedoLocation	;
	const int m_metallnessLocation;
	const int m_roughnessLocation;
	const int m_albedoSamplerLocation;
	const int m_normalSamplerLocation;
	const int m_roughnessMetallnessSamplerLocation;


	//textures
	const std::shared_ptr<Texture> m_albedoTexture;
	const std::shared_ptr<Texture> m_normalTexture;
	const std::shared_ptr<Texture> m_roughnessMetalnessTexture;

private:
	int m_directionalLightLocation;
	int m_projectionMatLocation;
	int m_viewMatLocation;
	int m_cameraPositionLocation;
	int m_shadowMapLocation;
	int m_shadowmapProjectionLocation;

}; 