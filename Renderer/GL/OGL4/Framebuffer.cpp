#include"Framebuffer.h"
#include"glad/glad.h"
#include<iostream>



Framebuffer::Framebuffer(
	const std::vector<std::shared_ptr<Texture>>& colorTextures,
	const std::shared_ptr<Texture>& depthTexture, 
	const AttachmentType attachmentType)
{
	glGenFramebuffers(1, &m_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
	unsigned int colorAttachmentID = 0;
	std::vector<unsigned int> attachments;
	for (const auto& colorTexture : colorTextures)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorAttachmentID, GL_TEXTURE_2D, colorTexture->getID(), 0);
		attachments.push_back(GL_COLOR_ATTACHMENT0 + colorAttachmentID);
		colorAttachmentID++;
	}
	glDrawBuffers(static_cast<unsigned int>( attachments.size()), attachments.data());
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture->getID(), 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		throw std::runtime_error("error creating famebuffer");
	}
}


Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &m_framebuffer);
}


void Framebuffer::bind()const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
}

void Framebuffer::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



void Framebuffer::setTexture(const std::unique_ptr<Texture>& colorAttachment, const std::unique_ptr<Texture>& depthAttachment)
{

	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachment->getID(), 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthAttachment->getID(), 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		throw std::runtime_error("error creating famebuffer");
	}
}

