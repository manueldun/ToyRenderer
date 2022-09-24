#include"Texture.h"
#include<glad/glad.h>
#include<iostream>
#include"stb_image.h"
Texture::Texture(std::string path)
{
	loadTexture(path);
}
Texture::Texture(unsigned int width, unsigned int height, TextureType textureType)
{
	m_height = height;
	m_width = width;
	m_depth = 1;
	switch (textureType)
	{
	case TextureType::COLOR:
		m_channels = 3;
		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_2D, m_textureID);
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_FLOAT, 0);
		break;
	case TextureType::DEPTH:
		m_channels = 1;
		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_2D, m_textureID);
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
		break;
	case TextureType::SINGLE_FLOAT:
		m_channels = 1;
		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_2D, m_textureID);
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT, 0);
		break;
	case TextureType::THREE_FLOAT:
		m_channels = 1;
		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_2D, m_textureID);
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, 0);
		break;
	case TextureType::CUBE_MAP:

		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);
		for (unsigned int i = 0; i < 6; i++)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, 0);
		}
		break;
	case TextureType::CUBE_MAP_DEPTH:

		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);
		for (unsigned int i = 0; i < 6; i++)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
		}
		break;
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

}
Texture::Texture(unsigned int width, unsigned int height, unsigned int channels, const std::vector<unsigned char>& data)
{
	m_width = width;
	m_height = height;
	m_depth = 1;
	m_channels = channels;

	switch (m_channels)
	{
	case 3:
		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_2D, m_textureID);
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());
		break;
	case 4:
		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_2D, m_textureID);
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
		break;
	default:
		break;
	}
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//float aniso = 0.0f;
	//glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &aniso);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, aniso);

}
Texture::Texture(const float red, const float green, const float blue)
{
	m_width = 1;
	m_height = 1;
	m_depth = 1;
	m_channels = 3;
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	const float texData[] = { red,green,blue };
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);

}

Texture::Texture(const TextureData& data)
{
	m_width = data.Width;
	m_height = data.Height;
	m_depth = data.Depth;
	m_channels = data.NumOfChannels;
	glGenTextures(1, &m_textureID);

	if (data.NumOfChannels == 2 && m_height == 1 && m_depth == 1)
	{
		glBindTexture(GL_TEXTURE_1D, m_textureID);
		glTexImage1D(GL_TEXTURE_1D, 0, GL_RG16, data.Width, 0, GL_RG, GL_UNSIGNED_SHORT, data.Data.data());

		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else
	{
		throw std::runtime_error("texture type not implemented");
	}
}
Texture::~Texture()
{
	glDeleteTextures(1, &m_textureID);
}
void Texture::bind(const unsigned int slot) const
{

	glActiveTexture(GL_TEXTURE0 + slot);
	if (m_height == 1 && m_width == 1 && m_depth == 1)
	{
		glBindTexture(GL_TEXTURE_2D, m_textureID);
	}
	else if (m_height == 1 && m_depth == 1)
	{
		glBindTexture(GL_TEXTURE_1D, m_textureID);
	}
	else  if (m_depth == 1){
		glBindTexture(GL_TEXTURE_2D, m_textureID);

	}
	else {
		throw std::runtime_error("texture bind not supported");
	}
}
void Texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);

}
unsigned int Texture::getID()const
{
	return m_textureID;
}
unsigned int Texture::getWidth() const
{
	return m_width;
}
unsigned int Texture::getHeight() const
{
	return m_height;
}
void Texture::loadTexture(std::string path)
{
	stbi_set_flip_vertically_on_load(1);
	int width, height, channels;
	unsigned char* img = stbi_load(path.c_str(), &width, &height, &channels, 0);
	if (img == NULL) {

		throw std::runtime_error("error opening image file");

	}
	m_width = width;
	m_height = height;
	m_channels = channels;
	switch (m_channels)
	{
	case 1:
		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_2D, m_textureID);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, img);
		break;
	case 3:
		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_2D, m_textureID);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
		break;
	case 4:
		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_2D, m_textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
		break;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);


}

TextureData::TextureData(
	const unsigned int width,
	const unsigned int height,
	const unsigned int depth,
	const unsigned int numsOfChannels,
	const std::vector<unsigned short> data)
	:
	Width(width),
	Height(height),
	Depth(depth),
	Data(data),
	NumOfChannels(numsOfChannels)
{
	if (data.size() != static_cast<unsigned long long>(height) * static_cast<unsigned long long>(width) * static_cast<unsigned long long>(numsOfChannels))
	{
		throw std::runtime_error("incorrect data size");
	}

}
