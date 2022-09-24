#include "PBRMaterial.h"

#include<glad/glad.h>
PBRMaterial::PBRMaterial(
	const float redAlbedo,
	const float greenAlbedo,
	const float blueAlbedo,
	const float roughness,
	const float metallness,
	const std::shared_ptr<Texture> albedoTexture, 
	const std::shared_ptr<Texture> normalTexture,
	const std::shared_ptr<Texture> roughnessMetalnessTexture,
	const std::shared_ptr<Shader> shader,
	const bool hasTangent)
	:
	m_redAlbedo(redAlbedo),
	m_greenAlbedo(greenAlbedo),
	m_blueAlbedo(blueAlbedo),
	m_roughness(roughness),
	m_metallness(metallness),
	m_shader(shader),
	m_hasTangent(hasTangent),
	m_albedoTexture(albedoTexture),
	m_normalTexture(normalTexture),
	m_roughnessMetalnessTexture(roughnessMetalnessTexture),
	m_albedoSamplerLocation(m_shader->getUniformLocation("albedoSampler")),
	m_normalSamplerLocation(m_shader->getUniformLocation("normalSampler")),
	m_roughnessMetallnessSamplerLocation(m_shader->getUniformLocation("metallicRoughness")),
	m_albedoLocation(m_shader->getUniformLocation("albedoConstant")),
	m_metallnessLocation(m_shader->getUniformLocation("metallicConstant")),
	m_roughnessLocation(m_shader->getUniformLocation("roughnessConstant"))

{
	/*
	m_albedoTexture = albedoTexture;
	m_normalTexture = normalTexture;
	m_roughnessMetalnessTexture = roughnessMetalnessTexture;

	
	m_albedoSamplerLocation = m_shader->getUniformLocation("albedoSampler");
	m_normalSamplerLocation = m_shader->getUniformLocation("normalSampler");
	m_roughnessMetallnessSamplerLocation = m_shader->getUniformLocation("metallicRoughness");

	*/
	m_projectionMatLocation = m_shader->getUniformLocation("projection");
	m_viewMatLocation = m_shader->getUniformLocation("view");
	m_directionalLightLocation = m_shader->getUniformLocation("directionalLight");//
	m_cameraPositionLocation = m_shader->getUniformLocation("cameraPosition");
	m_shadowmapProjectionLocation = m_shader->getUniformLocation("shadowmapProjection");
	m_shadowMapLocation = m_shader->getUniformLocation("shadowmap");
	/*

	m_albedoLocation = m_shader->getUniformLocation("albedoConstant");
	m_metallnessLocation = m_shader->getUniformLocation("metallicConstant");
	m_roughnessLocation = m_shader->getUniformLocation("roughnessConstant");*/

}
void PBRMaterial::bind()
{
	m_shader->bind();



	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
}

void PBRMaterial::setMaterialUniforms(std::shared_ptr<Shader> shader)
{
	shader->bind();
	int albedoSamplerLocation = shader->getUniformLocation("albedoSampler");
	m_albedoTexture->bind(0);
	glUniform1i(albedoSamplerLocation, 0);

	int normalSamplerLocation = shader->getUniformLocation("normalSampler");
	m_normalTexture->bind(1);
	glUniform1i(normalSamplerLocation, 1);

	int roughnessMetalnessSamplerLocation = shader->getUniformLocation("roughnessMetalnessSampler");
	m_roughnessMetalnessTexture->bind(2);
	glUniform1i(roughnessMetalnessSamplerLocation, 2);


	int albedoLocation = shader->getUniformLocation("albedo");
	glUniform3f(albedoLocation, m_redAlbedo, m_greenAlbedo, m_blueAlbedo);
	int metalnessLocation = shader->getUniformLocation("metalness");
	glUniform1f(metalnessLocation, m_metallness);
	int roughnessLocation = shader->getUniformLocation("roughness");
	glUniform1f(roughnessLocation, m_roughness);

}

std::shared_ptr<Shader> PBRMaterial::getShader()
{
	return m_shader;
}

std::shared_ptr<Texture> PBRMaterial::getNormalTexture()
{
	return m_normalTexture;
}

std::shared_ptr<Texture> PBRMaterial::getAlbedoTexture()
{
	return m_albedoTexture;
}

std::vector<float> PBRMaterial::getAlbedoConstants()
{
	return std::vector<float>({m_redAlbedo,m_greenAlbedo,m_blueAlbedo});
}

const int PBRMaterial::getUniformLocation(const std::string& uniformName)
{
	return m_shader->getUniformLocation(uniformName);
}

