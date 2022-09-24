#pragma once
#include<memory>
#include<vector>
#include"OGL4/Shader.h"
#include"OGL4/Texture.h"
class Material {
public:
	virtual void bind() = 0;
	virtual std::shared_ptr<Shader> getShader()=0;
	virtual void setMaterialUniforms(std::shared_ptr<Shader> shader) = 0;
	virtual std::shared_ptr<Texture> getNormalTexture() = 0;
	virtual std::shared_ptr<Texture> getAlbedoTexture() = 0;
	virtual std::vector<float> getAlbedoConstants() = 0;
	virtual const int getUniformLocation(const std::string& uniformName) = 0;
protected:
};
class PhongMaterial :public Material {
public:
	PhongMaterial() = delete;
	PhongMaterial(const PhongMaterial& material) = default;
	PhongMaterial(
		const float diffuseRed,
		const float diffuseGreen,
		const float diffuseBlue,
		const float specularRed,
		const float specularGreen,
		const float specularBlue,
		const float shinisess,
		const std::shared_ptr<Shader>& shader);
	PhongMaterial(
		const float specularRed,
		const float specularGreen,
		const float specularBlue,
		const float shinisess,
		const std::shared_ptr<Texture>& diffuseTexture,
		const std::shared_ptr<Shader>& shader);
	PhongMaterial(
		const float specularRed,
		const float specularGreen,
		const float specularBlue,
		const float shinisess,
		const std::shared_ptr<Texture>& diffuseTexture,
		const std::shared_ptr<Texture>& bumpTexture,
		const std::shared_ptr<Shader>& shader);
	PhongMaterial(
		const float diffuseRed,
		const float diffuseGreen,
		const float diffuseBlue,
		const float specularRed,
		const float specularGreen,
		const float specularBlue,
		const float shinisess,
		const std::shared_ptr<Texture>& bumpTexture,
		const std::shared_ptr<Shader>& shader);
	void setMaterialUniforms(std::shared_ptr<Shader> shader);
	void bind();
	std::shared_ptr<Shader> getShader();
	std::shared_ptr<Texture> getNormalTexture();
	std::shared_ptr<Texture> getAlbedoTexture();
	std::vector<float> getAlbedoConstants();
	const int getUniformLocation(const std::string& uniformName);
private:
	float m_diffuseRed;
	float m_diffuseGreen;
	float m_diffuseBlue;
	float m_specularRed;
	float m_specularGreen;
	float m_specularBlue;
	float m_shinines;
	int m_diffuseSamplerLocation;
	int m_bumpSamplerLocation;
	int m_diffuseColorLocation;
	int m_projectionMatLocation;
	int m_viewMatLocation;
	int m_directionalLightLocation;
	int m_specularColorLocation;
	int m_shininesLocation;
	int m_cameraPositionLocation;

	std::shared_ptr<Texture> m_bumpTexture;
	std::shared_ptr<Texture> m_diffuseTexture;

	std::shared_ptr<Shader> m_shader;

};