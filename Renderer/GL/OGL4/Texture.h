#pragma once
#include<string>
#include<vector>
enum class TextureType {
	COLOR,
	DEPTH,
	SINGLE_FLOAT,
	THREE_FLOAT,
	CUBE_MAP,
	CUBE_MAP_DEPTH
};
class TextureData {
public:
	TextureData(
		const unsigned int width,
		const unsigned int height,
		const unsigned int depth,
		const unsigned int NumOfChannels,
		const std::vector<unsigned short> data);

	const int Width;
	const int Height;
	const int Depth;
	const int NumOfChannels;
	const std::vector<unsigned short> Data;
private:
};
class Texture {
public:
	Texture() = delete;
	Texture(std::string path);
	Texture(unsigned int width, unsigned int height, TextureType textureType);
	Texture(unsigned int width, unsigned int height,unsigned int channels, const std::vector<unsigned char>& data);
	Texture(const float red, const float green, const float blue);
	Texture(const TextureData& data);
	~Texture();
	void bind(const unsigned int slot)const;
	void unbind()const;
	unsigned int getID()const;
	unsigned int getWidth()const;
	unsigned int getHeight()const;
private:
	void loadTexture(std::string path);
	unsigned int m_width;
	unsigned int m_height;
	unsigned int m_depth;
	unsigned int m_channels;
	unsigned int m_textureID;
};