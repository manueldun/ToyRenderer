#pragma once
#include<string>
#include<unordered_map>
struct Pipeline {
	std::string vertexShader;
	std::string geometryShader;
	std::string fragmentShader;
};
const unsigned int SOLID = 0;
const unsigned int DIFFUSE_TEXTURED = 1;
const unsigned int BUMP_TEXTURED = 2;

class Shader {
public:
	Shader() = delete;
	Shader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
	//Shader(const char* path, unsigned int shaderTypeMask);
	Shader(const char* path);
	~Shader();
	void bind() const;
	int getUniformLocation(const std::string& uniformName) const;
private:
	mutable std::unordered_map<std::string, int> m_uniformLocationMap;
	unsigned int m_program;
	void compileAndLink(Pipeline pipeline);
	//const Pipeline separateShaders(const char* path, unsigned int shaderTypeMask) const;
	const Pipeline separateShaders(const char* path) const;
};