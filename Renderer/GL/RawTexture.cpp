#include "RawTexture.h"

RawTexture::RawTexture(std::vector<unsigned char> data, unsigned int width, unsigned int height, unsigned int components)
	:m_data(data),
	m_width(width),
	m_height(height),
	m_components(components)
{
}


void RawTexture::loadToGPU()
{
	m_gpuTexture = std::make_shared<Texture>(m_width,m_height,m_components,m_data);
}

void RawTexture::bind(const unsigned int slot) const
{
	m_gpuTexture->bind(slot);
}