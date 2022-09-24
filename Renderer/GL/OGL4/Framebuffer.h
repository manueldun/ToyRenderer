#pragma once
#include<memory>
#include"Texture.h"
enum class AttachmentType{
	STANDARD,
	CUBE_MAP
};
class Framebuffer {
public:
	Framebuffer(
		const std::vector<std::shared_ptr<Texture>>& colorTextures,
		const std::shared_ptr<Texture>& depthTexture,
		const AttachmentType attachmentType);
	~Framebuffer();
	void bind()const;
	static void unbind();
	void setTexture(const std::unique_ptr<Texture>& colorAttachment, const std::unique_ptr<Texture>& depthAttachment);
private:
	unsigned int m_framebuffer;
};