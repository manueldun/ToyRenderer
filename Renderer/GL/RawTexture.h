#pragma once
#include<memory>
#include<vector>
#include"Texture.h"
class RawTexture {
public:
	RawTexture() = delete;
	RawTexture(std::vector<unsigned char> data, unsigned int width, unsigned int height, unsigned int components);
	void loadToGPU();
	void bind(const unsigned int slot) const;
private:
	std::vector<unsigned char> m_data;
	unsigned int m_width=0;
	unsigned int m_height=0;
	unsigned int m_components=0;
	//GPU
	std::shared_ptr<Texture> m_gpuTexture;
};