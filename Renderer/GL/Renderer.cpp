#include "Renderer.h"
#include<glad/glad.h>
#include<vector>
#include"Mesh.h"
#include"imgui.h"
#include<glm/gtc/type_ptr.hpp>
#include<cmath>
#include<numbers>
#include<iostream>
#include <cstdlib>
Renderer::Renderer(unsigned int width, unsigned int height)
	:m_viewportWidth(width),
	m_viewportHeight(height),
	RandomTextureData(generateRandomSamplePoints(100))
{
	clearAll();
}
void Renderer::render(
	const VertexArray& vertexArray,
	const IndexBuffer& indexBuffer,
	const RenderType& renderType)const
{
	vertexArray.bind();
	indexBuffer.bind();
	glEnableVertexAttribArray(0);
	switch (renderType)
	{
	case RenderType::LINE_LOOP:
		glDrawElements(GL_LINE_LOOP, indexBuffer.getCount(), GL_UNSIGNED_INT, 0);
		break;
	case RenderType::LINES:
		glDrawElements(GL_LINES, indexBuffer.getCount(), GL_UNSIGNED_INT, 0);
		break;
	}
}

void Renderer::render(
	const float* p,
	const float* v,
	const float* lightDirection,
	const float* cameraPosition,
	const Mesh& mesh,
	const float* lightMatrix) const
{

	Framebuffer::unbind();
	std::vector<SubMesh> subMeshes = mesh.getSubMeshes();
	glViewport(0, 0, m_viewportWidth, m_viewportHeight);
	for (const auto& subMesh : subMeshes)
	{
		mesh.getVertexArray()->bind();

		subMesh.getMaterial()->setMaterialUniforms(subMesh.getMaterial()->getShader());


		int projectionMatLocation = subMesh.getMaterial()->getShader()->getUniformLocation("projection");
		glUniformMatrix4fv(projectionMatLocation, 1, false, p);

		int viewMatLocation = subMesh.getMaterial()->getShader()->getUniformLocation("view");
		glUniformMatrix4fv(viewMatLocation, 1, false, v);

		int directionalLightLocation = subMesh.getMaterial()->getShader()->getUniformLocation("directionalLight");//


		int cameraPositionLocation = subMesh.getMaterial()->getShader()->getUniformLocation("cameraPosition");
		glUniform3fv(cameraPositionLocation, 1, cameraPosition);


		int shadowmapProjectionLocation = subMesh.getMaterial()->getShader()->getUniformLocation("shadowmapProjection");
		glUniformMatrix4fv(shadowmapProjectionLocation, 1, false, lightMatrix);

		int shadowMapLocation = subMesh.getMaterial()->getShader()->getUniformLocation("shadowmap");
		ShadowDepth->bind(3);
		glUniform1i(shadowMapLocation, 3);

		IndirectLightTexture->bind(4);
		glUniform1i(subMesh.getMaterial()->getUniformLocation("indirectLight"), 4);
		subMesh.getIndexBufferID()->bind();

		glDrawElements(GL_TRIANGLES, subMesh.getIndexBufferID()->getCount(), GL_UNSIGNED_SHORT, 0);
	}
}

void Renderer::renderShadowMap(
	const float* p,
	const Mesh& mesh) const
{

	const std::shared_ptr<Shader> rsmShader = ShaderType.at("Reflective ShadowMap");
	ShadowMapFramebuffer->bind();
	unsigned int h = PositionTexture->getHeight();
	unsigned int w = PositionTexture->getWidth();
	glViewport(0, 0, w, h);
	std::vector<SubMesh> subMeshes = mesh.getSubMeshes();
	for (const auto& subMesh : subMeshes)
	{
		mesh.getVertexArray()->bind();

		subMesh.getIndexBufferID()->bind();
		rsmShader->bind();
		glUniformMatrix4fv(rsmShader->getUniformLocation("projection"), 1, false, p);

		subMesh.getMaterial()->getAlbedoTexture()->bind(0);
		glUniform1i(rsmShader->getUniformLocation("albedoSampler"), 0);

		subMesh.getMaterial()->getNormalTexture()->bind(1);
		glUniform1i(rsmShader->getUniformLocation("normalMapSampler"), 1);

		std::vector<float> albedoConstant = subMesh.getMaterial()->getAlbedoConstants();
		glUniform3fv(rsmShader->getUniformLocation("albedoConstant"), 1, albedoConstant.data());

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glDrawElements(GL_TRIANGLES, subMesh.getIndexBufferID()->getCount(), GL_UNSIGNED_SHORT, 0);
	}
}

void Renderer::renderRMS(const float* projection, const float* view, const float* lightProjection, const Mesh& mesh, float span) const
{

	const std::shared_ptr<Shader> indirectLightShader = ShaderType.at("Indirect Pass");
	IndirectLighFramebuffer->bind();
	unsigned int h = IndirectLightTexture->getHeight();
	unsigned int w = IndirectLightTexture->getWidth();
	glViewport(0, 0, w, h);
	std::vector<SubMesh> subMeshes = mesh.getSubMeshes();
	for (const auto& subMesh : subMeshes)
	{
		mesh.getVertexArray()->bind();

		subMesh.getIndexBufferID()->bind();
		indirectLightShader->bind();
		glUniformMatrix4fv(indirectLightShader->getUniformLocation("projection"), 1, false, projection);
		glUniformMatrix4fv(indirectLightShader->getUniformLocation("view"), 1, false, view);
		glUniformMatrix4fv(indirectLightShader->getUniformLocation("lightProjection"), 1, false, lightProjection);

		subMesh.getMaterial()->getAlbedoTexture()->bind(0);
		glUniform1i(indirectLightShader->getUniformLocation("albedoTexture"), 0);

		subMesh.getMaterial()->getNormalTexture()->bind(1);
		glUniform1i(indirectLightShader->getUniformLocation("normalTexture"), 1);

		std::vector<float> albedoConstant = subMesh.getMaterial()->getAlbedoConstants();
		glUniform3fv(indirectLightShader->getUniformLocation("albedoConstant"), 1, albedoConstant.data());

		PositionTexture->bind(2);
		glUniform1i(indirectLightShader->getUniformLocation("positionRSM"), 2);

		AlbedoTexture->bind(3);
		glUniform1i(indirectLightShader->getUniformLocation("albedoRSM"), 3);

		NormalTexture->bind(4);
		glUniform1i(indirectLightShader->getUniformLocation("normalRSM"), 4);

		//RandomTextureData->bind(5);
		//glUniform1i(indirectLightShader->getUniformLocation("samplePoints"), 5);

		glUniform1f(indirectLightShader->getUniformLocation("sampleSpan"), span);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glDrawElements(GL_TRIANGLES, subMesh.getIndexBufferID()->getCount(), GL_UNSIGNED_SHORT, 0);
	}
}

void Renderer::renderGBuffer(const float* projection, const float* view, const Mesh& mesh) const
{

	const std::shared_ptr<Shader> gBufferShader = ShaderType.at("GBuffer");
	GBufferFramebuffer->bind();
	std::vector<SubMesh> subMeshes = mesh.getSubMeshes();
	glViewport(0, 0, m_viewportWidth, m_viewportHeight);
	for (const auto& subMesh : subMeshes)
	{
		mesh.getVertexArray()->bind();

		subMesh.getMaterial()->setMaterialUniforms(gBufferShader);

		glUniformMatrix4fv(gBufferShader->getUniformLocation("projection"), 1, false, projection);
		glUniformMatrix4fv(gBufferShader->getUniformLocation("view"), 1, false, view);


		subMesh.getIndexBufferID()->bind();

		glDrawElements(GL_TRIANGLES, subMesh.getIndexBufferID()->getCount(), GL_UNSIGNED_SHORT, 0);
	}
}

void Renderer::renderDeferredPass(
	const float* lightMatrix,
	const float* lightDirection,
	const float* cameraPosition) const
{
	Framebuffer::unbind();
	std::shared_ptr<Shader> deferredPassShader = ShaderType.at("Deferred Pass");
	deferredPassShader->bind();
	glUniformMatrix4fv(deferredPassShader->getUniformLocation("lightMatrix"), 1, false, lightMatrix);
	GBufferPosition->bind(0);
	glUniform1i(deferredPassShader->getUniformLocation("gPosition"), 0);
	GBufferAlbedo->bind(1);
	glUniform1i(deferredPassShader->getUniformLocation("gAlbedo"), 1);
	GBufferNormal->bind(2);
	glUniform1i(deferredPassShader->getUniformLocation("gNormal"), 2);
	GBufferMetallicPoughness->bind(3);
	glUniform1i(deferredPassShader->getUniformLocation("gMetallicPoughness"), 3);
	PositionTexture->bind(4);
	glUniform1i(deferredPassShader->getUniformLocation("positionRSM"), 4);
	AlbedoTexture->bind(5);
	glUniform1i(deferredPassShader->getUniformLocation("albedoRSM"), 5);
	NormalTexture->bind(6);
	glUniform1i(deferredPassShader->getUniformLocation("normalRSM"), 6);
	ShadowDepth->bind(7);
	glUniform1i(deferredPassShader->getUniformLocation("shadowmap"), 7);

	glUniform3fv(deferredPassShader->getUniformLocation("lightDirection"), 1, lightDirection);

	glUniform3fv(deferredPassShader->getUniformLocation("cameraPosition"), 1, cameraPosition);


	glDrawArrays(GL_TRIANGLES, 0, 3);
}

std::shared_ptr<const Texture> Renderer::generateRandomSamplePoints(unsigned int numOfPoints)
{
	std::vector<unsigned short> data;
	data.reserve(static_cast<unsigned long long>(numOfPoints) * 2);
	for (unsigned int i = 0; i < numOfPoints * 2; ++i)
	{
		data.push_back(std::rand());
	}
	return std::make_shared<const Texture>(TextureData(numOfPoints, 1, 1, 2, data));
}



void Renderer::clearAll() const
{
	Framebuffer::unbind();
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ShadowMapFramebuffer->bind();
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	IndirectLighFramebuffer->bind();
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GBufferFramebuffer->bind();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}


