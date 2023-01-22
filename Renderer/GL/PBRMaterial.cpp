#include "PBRMaterial.h"

#include<glad/glad.h>
#include"Renderer.h"
PBRMaterial::PBRMaterial(
	const float redAlbedo,
	const float greenAlbedo,
	const float blueAlbedo,
	const float roughness,
	const float metalness,
	const RawTexture& albedoRawTexture, 
	const RawTexture& normalRawTexture,
	const RawTexture& roughnessMetalnessRawTexture,
	const bool hasTangent)
	:
	m_redAlbedo(redAlbedo),
	m_greenAlbedo(greenAlbedo),
	m_blueAlbedo(blueAlbedo),
	m_roughness(roughness),
	m_metalness(metalness),
	m_hasTangent(hasTangent),
	m_albedoRawTexture(albedoRawTexture),
	m_normalRawTexture(normalRawTexture),
	m_roughnessMetalnessRawTexture(roughnessMetalnessRawTexture)

{


}



void PBRMaterial::loadToGPU()
{
	m_albedoRawTexture.loadToGPU();
	m_normalRawTexture.loadToGPU();
	m_roughnessMetalnessRawTexture.loadToGPU();
}




void PBRMaterial::setMaterialUniforms(std::shared_ptr<Shader> shader) const
{
	glUniform3f(shader->getUniformLocation("albedo"), m_redAlbedo, m_greenAlbedo, m_blueAlbedo);
	glUniform1f(shader->getUniformLocation("metalness"), m_metalness);
	glUniform1f(shader->getUniformLocation("roughness"), m_roughness);

	m_albedoRawTexture.bind(0);
	glUniform1i(shader->getUniformLocation("albedoSampler"),0);
	m_roughnessMetalnessRawTexture.bind(1);
	glUniform1i(shader->getUniformLocation("roughnessMetalnessSampler"),1);
	m_normalRawTexture.bind(2);
	glUniform1i(shader->getUniformLocation("normalSampler"),2);
}

const RawTexture& PBRMaterial::getAlbedoTexture() const
{

	return m_albedoRawTexture;
	
}

const RawTexture& PBRMaterial::getNormalTexture() const
{
	return m_normalRawTexture;
}

const RawTexture& PBRMaterial::getRoughnessMetalnessTexture() const
{
	return m_roughnessMetalnessRawTexture;
}

std::vector<float> PBRMaterial::getAlbedoConstants() const
{
	return { m_redAlbedo,m_greenAlbedo,m_blueAlbedo };
}
