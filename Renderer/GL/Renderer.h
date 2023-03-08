#pragma once
#include<memory>
#include"OGL4/Shader.h"
#include"Mesh.h"
#include"OGL4/Framebuffer.h"
#include"OGL4/IndexBuffer.h"
#include"VertexArray.h"
#include<glm/glm.hpp>
#include<unordered_map>
#include<string>
enum class RenderType {
	LINE_LOOP,
	LINES
};
struct RenderBufferTexture {
	Texture colorBuffer;
	Texture depthBuffer;
};
enum ShaderType {
	REFLECTIVE_SHADOWMAP,
	INDIRECT_PASS,
	PBR_SHADER,
	GBUFFER,
	DEFFERRED_PASS
};
class Renderer {
public:
	Renderer(unsigned int width, unsigned int height);
	void render(
		const VertexArray& VertexArray,
		const IndexBuffer& indexBuffer,
		const RenderType& renderType)const;
	void render(
		const float* p,
		const float* v,
		const float* lightDirection,
		const float* cameraPosition,
		const Mesh& mesh,
		const float* lightMatrix) const;
	void clearAll() const;
	void renderShadowMap(
		const float* projection,
		const Mesh& mesh) const;
	const unsigned int m_viewportWidth;
	const unsigned int m_viewportHeight;


	/*
	const std::vector<std::shared_ptr<Shader>> Shaders = std::vector<std::shared_ptr<Shader>>({
		std::make_shared<Shader>("./GlobalIllumination/GL/Shaders/Shader.glsl",SOLID),
		std::make_shared<Shader>("./GlobalIllumination/GL/Shaders/Shader.glsl",DIFFUSE_TEXTURED),
		std::make_shared<Shader>("./GlobalIllumination/GL/Shaders/Shader.glsl",BUMP_TEXTURED),
		std::make_shared<Shader>("./GlobalIllumination/GL/Shaders/Shader.glsl",DIFFUSE_TEXTURED | BUMP_TEXTURED)
		});//old non pbr shaders*/
	void renderRMS(
		const float* projection,
		const float* view,
		const float* lightProjection,
		const Mesh& mesh,
		float span) const;
	void renderGBuffer(
		const float* projection,
		const float* view,
		const Mesh& mesh
	) const;
	void renderDeferredPass(
		const float* lightMatrix,
		const float* lightDirection,
		const float* cameraPosition) const;
	const unsigned int ReflectiveShadowMapSize = 2048;
	const std::shared_ptr<Texture> PositionTexture =
		std::make_shared<Texture>(ReflectiveShadowMapSize, ReflectiveShadowMapSize, TextureType::THREE_FLOAT);
	const std::shared_ptr<Texture> NormalTexture =
		std::make_shared<Texture>(ReflectiveShadowMapSize, ReflectiveShadowMapSize, TextureType::THREE_FLOAT);
	const std::shared_ptr<Texture> AlbedoTexture =
		std::make_shared<Texture>(ReflectiveShadowMapSize, ReflectiveShadowMapSize, TextureType::THREE_FLOAT);
	const std::shared_ptr<Texture> ShadowDepth =
		std::make_shared<Texture>(ReflectiveShadowMapSize, ReflectiveShadowMapSize, TextureType::DEPTH);

	const std::shared_ptr<Framebuffer> ShadowMapFramebuffer =
		std::make_shared<Framebuffer>(
			std::vector<std::shared_ptr<Texture>>({ PositionTexture ,NormalTexture,AlbedoTexture }),
			ShadowDepth,
			AttachmentType::STANDARD);

	const unsigned int IndirectLightWidth = m_viewportWidth;
	const unsigned int IndirectLightHeight = m_viewportHeight;
	const std::shared_ptr<Texture> IndirectLightTexture =
		std::make_shared<Texture>(IndirectLightWidth, IndirectLightHeight, TextureType::THREE_FLOAT);
	const std::shared_ptr<Texture> IndirectShadowDepth =
		std::make_shared<Texture>(IndirectLightWidth, IndirectLightHeight, TextureType::DEPTH);

	const std::shared_ptr<Framebuffer> IndirectLighFramebuffer =
		std::make_shared<Framebuffer>(
			std::vector<std::shared_ptr<Texture>>({ IndirectLightTexture }),
			IndirectShadowDepth,
			AttachmentType::STANDARD);

	const std::shared_ptr<Texture> GBufferPosition =
		std::make_shared<Texture>(m_viewportWidth, m_viewportHeight, TextureType::THREE_FLOAT);
	const std::shared_ptr<Texture> GBufferAlbedo =
		std::make_shared<Texture>(m_viewportWidth, m_viewportHeight, TextureType::THREE_FLOAT);
	const std::shared_ptr<Texture> GBufferNormal =
		std::make_shared<Texture>(m_viewportWidth, m_viewportHeight, TextureType::THREE_FLOAT);
	const std::shared_ptr<Texture> GBufferMetallicPoughness =
		std::make_shared<Texture>(m_viewportWidth, m_viewportHeight, TextureType::THREE_FLOAT);
	const std::shared_ptr<Texture> GBufferDepth =
		std::make_shared<Texture>(m_viewportWidth, m_viewportHeight, TextureType::DEPTH);

	const std::shared_ptr<Framebuffer> GBufferFramebuffer =
		std::make_shared<Framebuffer>(
			std::vector<std::shared_ptr<Texture>>({ GBufferPosition,GBufferAlbedo,GBufferNormal,GBufferMetallicPoughness }),
			GBufferDepth,
			AttachmentType::STANDARD
			);
	static std::shared_ptr<Shader> getShader(ShaderType shaderType);
private:
	static bool initialized;
	static std::unordered_map<ShaderType, std::shared_ptr<Shader>> shaders;

};
