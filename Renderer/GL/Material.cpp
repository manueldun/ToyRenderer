#include"Material.h"
#include<glad/glad.h>
#include<iostream>

PhongMaterial::PhongMaterial(
	const float diffuseRed,
	const float diffuseGreen,
	const float diffuseBlue,
	const float specularRed,
	const float specularGreen,
	const float specularBlue,
	const float shinisess,
	const std::shared_ptr<Shader>& shader )
	:
	m_diffuseRed(diffuseRed),
	m_diffuseGreen(diffuseGreen),
	m_diffuseBlue(diffuseBlue),
	m_specularRed(specularRed),
	m_specularGreen(specularGreen),
	m_specularBlue(specularBlue),
	m_shinines(shinisess)
{

	m_shader = shader;
	m_diffuseTexture = nullptr;
	m_diffuseColorLocation = shader->getUniformLocation("diffuseColor");
	m_diffuseSamplerLocation = -1;

	m_projectionMatLocation = shader->getUniformLocation("projection");
	m_viewMatLocation = shader->getUniformLocation("view");
	m_directionalLightLocation = shader->getUniformLocation("directionalLight");
	m_shininesLocation = shader->getUniformLocation("shininess");
	m_specularColorLocation = shader->getUniformLocation("specularity");
	m_cameraPositionLocation = shader->getUniformLocation("cameraPosition");

}

PhongMaterial::PhongMaterial(
	const float specularRed,
	const float specularGreen,
	const float specularBlue,
	const float shinisess, 
	const std::shared_ptr<Texture>& diffuseTexture,
	const std::shared_ptr<Shader>& shader)
	:
	m_diffuseRed(0.0f),
	m_diffuseGreen(0.0f),
	m_diffuseBlue(0.0f),
	m_specularRed(specularRed),
	m_specularGreen(specularGreen),
	m_specularBlue(specularBlue),
	m_shinines(shinisess)
{
	m_shader = shader;
	m_diffuseTexture = diffuseTexture;
	m_bumpTexture = nullptr;

	m_diffuseColorLocation = -1;
	m_diffuseSamplerLocation = shader->getUniformLocation("colorSampler");
	m_projectionMatLocation = shader->getUniformLocation("projection");
	m_viewMatLocation = shader->getUniformLocation("view");
	m_directionalLightLocation = shader->getUniformLocation("directionalLight");
	m_shininesLocation = shader->getUniformLocation("shininess");
	m_specularColorLocation = shader->getUniformLocation("specularity");
	m_cameraPositionLocation = shader->getUniformLocation("cameraPosition");


}

PhongMaterial::PhongMaterial(
	const float specularRed,
	const float specularGreen,
	const float specularBlue,
	const float shinisess, 
	const std::shared_ptr<Texture>& diffuseTexture, 
	const std::shared_ptr<Texture>& bumpTexture,
	const std::shared_ptr<Shader>& shader)
	:
	m_diffuseRed(1.0f),
	m_diffuseGreen(0.0f),
	m_diffuseBlue(1.0f),
	m_specularRed(specularRed),
	m_specularGreen(specularGreen),
	m_specularBlue(specularBlue),
	m_shinines(shinisess)
{
	m_shader = shader;
	m_diffuseTexture = diffuseTexture;
	m_bumpTexture = bumpTexture;

	m_diffuseColorLocation = -1;
	m_diffuseSamplerLocation = shader->getUniformLocation("colorSampler");
	m_bumpSamplerLocation = shader->getUniformLocation("bumpSampler");
	m_projectionMatLocation = shader->getUniformLocation("projection");
	m_viewMatLocation = shader->getUniformLocation("view");
	m_directionalLightLocation = shader->getUniformLocation("directionalLight");
	m_shininesLocation = shader->getUniformLocation("shininess");
	m_specularColorLocation = shader->getUniformLocation("specularity");
	m_cameraPositionLocation = shader->getUniformLocation("cameraPosition");
}

PhongMaterial::PhongMaterial(
	const float specularRed,
	const float specularGreen,
	const float specularBlue,
	const float shinisess, 
	const float diffuseRed, 
	const float diffuseGreen, 
	const float diffuseBlue, 
	const std::shared_ptr<Texture>& bumpTexture,
	const std::shared_ptr<Shader>& shader)

	:m_diffuseRed(diffuseRed),
	m_diffuseGreen(diffuseGreen),
	m_diffuseBlue(diffuseBlue),
	m_specularRed(specularRed),
	m_specularGreen(specularGreen),
	m_specularBlue(specularBlue),
	m_shinines(shinisess)
{

	m_shader = shader;
	m_diffuseTexture = nullptr;
	m_bumpTexture = bumpTexture;

	m_diffuseColorLocation = -1;
	m_diffuseSamplerLocation = -1;
	m_bumpSamplerLocation = shader->getUniformLocation("bumpSampler");
	m_projectionMatLocation = shader->getUniformLocation("projection");
	m_viewMatLocation = shader->getUniformLocation("view");
	m_directionalLightLocation = shader->getUniformLocation("directionalLight");
	m_shininesLocation = shader->getUniformLocation("shininess");
	m_specularColorLocation = shader->getUniformLocation("specularity");
	m_cameraPositionLocation = shader->getUniformLocation("cameraPosition");
}

void PhongMaterial::setMaterialUniforms(std::shared_ptr<Shader> shader)
{
	throw std::runtime_error("phong material setMaterialUniform not implemented fully");
	shader->bind();
	if (m_diffuseTexture == nullptr && m_bumpTexture == nullptr)
	{
		glUniform3f(m_diffuseColorLocation, m_diffuseRed, m_diffuseGreen, m_diffuseBlue);
		glUniform3f(m_specularColorLocation, m_specularRed, m_specularGreen, m_specularBlue);
	}
	else if (m_diffuseTexture != nullptr && m_bumpTexture == nullptr)
	{
		m_diffuseTexture->bind(0);
		glUniform1i(m_diffuseSamplerLocation, 0);
		glUniform3f(m_specularColorLocation, m_specularRed, m_specularGreen, m_specularBlue);
		glUniform1f(m_shininesLocation, m_shinines);
	}
	else if (m_diffuseTexture == nullptr && m_bumpTexture != nullptr)
	{

		glUniform3f(m_diffuseColorLocation, m_diffuseRed, m_diffuseGreen, m_diffuseBlue);
		m_bumpTexture->bind(1);
		glUniform1i(m_bumpSamplerLocation, 1);
		glUniform3f(m_specularColorLocation, m_specularRed, m_specularGreen, m_specularBlue);
		glUniform1f(m_shininesLocation, m_shinines);
	}
	else {

		m_diffuseTexture->bind(0);
		glUniform1i(m_diffuseSamplerLocation, 0);
		m_bumpTexture->bind(1);
		glUniform1i(m_bumpSamplerLocation, 1);
		glUniform3f(m_specularColorLocation, m_specularRed, m_specularGreen, m_specularBlue);
		glUniform1f(m_shininesLocation, m_shinines);
	}

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	if (m_bumpTexture != nullptr)
	{
		glEnableVertexAttribArray(3);
	}
	else
	{

		glDisableVertexAttribArray(3);
	}
}


void PhongMaterial::bind()
{
	m_shader->bind();
	
}

std::shared_ptr<Shader> PhongMaterial::getShader()
{
	return m_shader;
}

std::shared_ptr<Texture> PhongMaterial::getNormalTexture()
{
	return std::make_shared<Texture>(0.0f,0.0f,1.0f);
}

std::shared_ptr<Texture> PhongMaterial::getAlbedoTexture()
{
	return m_diffuseTexture;
}

std::vector<float> PhongMaterial::getAlbedoConstants()
{
	return std::vector<float>({ m_diffuseRed ,m_diffuseGreen,m_diffuseBlue});
}

const int PhongMaterial::getUniformLocation(const std::string& uniformName)
{
	return m_shader->getUniformLocation(uniformName);
}
