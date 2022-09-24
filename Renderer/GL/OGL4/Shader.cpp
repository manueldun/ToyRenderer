#include"Shader.h"
#include<glad/glad.h>
#include<string>
#include<vector>
#include<iostream>
#include <fstream>
Shader::Shader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
{
	Pipeline pipeline;
	pipeline.vertexShader = vertexShaderSource;
	pipeline.fragmentShader = fragmentShaderSource;
	compileAndLink(pipeline);
}

Shader::~Shader()
{
	glDeleteProgram(m_program);
}



void Shader::bind() const
{
	glUseProgram(m_program);

}
Shader::Shader(const char* path)
{
	Pipeline pipeline = separateShaders(path);
	compileAndLink(pipeline);
}
/*
Shader::Shader(const char* path, unsigned int shaderTypeMask)
{
	Pipeline pipeline = separateShaders(path, shaderTypeMask);
	compileAndLink(pipeline);
}


const Pipeline Shader::separateShaders(const char* path, unsigned int shaderTypeMask) const
{
	Pipeline pipeline;
	std::ifstream file(path);
	if (!file.good())
	{
		throw std::runtime_error("error opening shader file");
	}
	if (file.is_open())
	{
		enum class ShaderType {
			NULL_TYPE,
			VERTEX_SHADER,
			GEOMETRY_SHADER,
			FRAGMENT_SHADER
		};
		ShaderType shaderType = ShaderType::NULL_TYPE;
		std::string line;
		while (getline(file, line))
		{
			if (line.compare("#VertexShader") == 0)
			{
				shaderType = ShaderType::VERTEX_SHADER;
			}
			else if (line.compare("#FragmentShader") == 0)
			{
				shaderType = ShaderType::FRAGMENT_SHADER;
			}
			else if (line.compare("#End") == 0)
			{
				shaderType = ShaderType::NULL_TYPE;
			}
			else
			{
				if (shaderType == ShaderType::VERTEX_SHADER)
				{
					pipeline.vertexShader += line + "\n";
				}
				if (shaderType == ShaderType::GEOMETRY_SHADER)
				{
					pipeline.geometryShader += line + "\n";
				}
				if (shaderType == ShaderType::FRAGMENT_SHADER)
				{
					pipeline.fragmentShader += line + "\n";
				}
			}
		}
	}
	const std::string versionsAndDefinesForSolid =
		"#version 400\n"
		"#define DIFFUSE_SOLID 1\n"
		"#define DIFFUSE_TEXTURED 0\n"
		"#define BUMP_MAP 0\n";
	const std::string versionsAndDefinesForDiffuseTexture =
		"#version 400\n"
		"#define DIFFUSE_SOLID 0\n"
		"#define DIFFUSE_TEXTURED 1\n"
		"#define BUMP_MAP 0\n";
	const std::string versionsAndDefinesForBumpTexture =
		"#version 400\n"
		"#define DIFFUSE_SOLID 0\n"
		"#define DIFFUSE_TEXTURED 0\n"
		"#define BUMP_MAP 1\n";
	const std::string versionsAndDefinesForColorBumpTexture =
		"#version 400\n"
		"#define DIFFUSE_SOLID 0\n"
		"#define DIFFUSE_TEXTURED 1\n"
		"#define BUMP_MAP 1\n";
	std::string versionsAndDefineToUse;
	switch (shaderTypeMask)
	{
	case 0:
		versionsAndDefineToUse = versionsAndDefinesForSolid;
		break;
	case 1:
		versionsAndDefineToUse = versionsAndDefinesForDiffuseTexture;
		break;
	case 2:
		versionsAndDefineToUse = versionsAndDefinesForBumpTexture;
		break;
	case 3:
		versionsAndDefineToUse = versionsAndDefinesForColorBumpTexture;
		break;
	}
	pipeline.vertexShader = versionsAndDefineToUse + pipeline.vertexShader;
	pipeline.fragmentShader = versionsAndDefineToUse + pipeline.fragmentShader;
	return pipeline;
}
*/
const Pipeline Shader::separateShaders(const char* path) const
{
	Pipeline pipeline;
	std::ifstream file(path);
	if (!file.good())
	{
		throw std::runtime_error("error opening shader file");
	}
	if (file.is_open())
	{
		enum class ShaderType {
			NULL_TYPE,
			VERTEX_SHADER,
			GEOMETRY_SHADER,
			FRAGMENT_SHADER
		};
		ShaderType shaderType = ShaderType::NULL_TYPE;
		std::string line;
		while (getline(file, line))
		{
			if (line.compare("#VertexShader") == 0)
			{
				shaderType = ShaderType::VERTEX_SHADER;
			}
			else if (line.compare("#FragmentShader") == 0)
			{
				shaderType = ShaderType::FRAGMENT_SHADER;
			}
			else if (line.compare("#End") == 0)
			{
				shaderType = ShaderType::NULL_TYPE;
			}
			else
			{
				if (shaderType == ShaderType::VERTEX_SHADER)
				{
					pipeline.vertexShader += line + "\n";
				}
				if (shaderType == ShaderType::GEOMETRY_SHADER)
				{
					pipeline.geometryShader += line + "\n";
				}
				if (shaderType == ShaderType::FRAGMENT_SHADER)
				{
					pipeline.fragmentShader += line + "\n";
				}
			}
		}
	}
	const std::string glslVersion = "#version 400\n";
	pipeline.vertexShader = glslVersion + pipeline.vertexShader;
	pipeline.fragmentShader = glslVersion + pipeline.fragmentShader;
	return pipeline;
}

void Shader::compileAndLink(Pipeline pipeline)
{
	GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
	const char* cvertexShaderSource = pipeline.vertexShader.c_str();
	glShaderSource(vertexShaderObject, 1, &cvertexShaderSource, nullptr);
	glCompileShader(vertexShaderObject);
	GLint success = 0;
	glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLint logSize = 0;
		glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &logSize);
		std::vector<GLchar> errorLog(logSize);
		glGetShaderInfoLog(vertexShaderObject, logSize, &logSize, &errorLog[0]);

		glDeleteShader(vertexShaderObject);
		std::cout << errorLog.data() << std::endl;
		throw std::runtime_error("Vertex shader compile error");
	}

	GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	const char* cfragmentShaderSource = pipeline.fragmentShader.c_str();
	glShaderSource(fragmentShaderObject, 1, &cfragmentShaderSource, nullptr);
	glCompileShader(fragmentShaderObject);
	success = 0;
	glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLint logSize = 0;
		glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH, &logSize);
		std::vector<GLchar> errorLog(logSize);
		glGetShaderInfoLog(fragmentShaderObject, logSize, &logSize, &errorLog[0]);

		glDeleteShader(fragmentShaderObject);
		std::cout << errorLog.data() << std::endl;
		throw std::runtime_error("Fragment shader compile error");
	}

	m_program = glCreateProgram();
	glAttachShader(m_program, vertexShaderObject);
	glAttachShader(m_program, fragmentShaderObject);

	glLinkProgram(m_program);

	GLint isLinked = 0;
	glGetProgramiv(m_program, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(m_program, maxLength, &maxLength, &infoLog[0]);
		std::cout << infoLog.data() << std::endl;

		// We don't need the program anymore.
		glDeleteProgram(m_program);
		// Don't leak shaders either.
		glDeleteShader(vertexShaderObject);
		glDeleteShader(fragmentShaderObject);

		// Use the infoLog as you see fit.

		// In this simple program, we'll just leave

		throw std::runtime_error("shader link error");
	}
	// Always detach shaders after a successful link.
	glDetachShader(m_program, vertexShaderObject);
	glDetachShader(m_program, fragmentShaderObject);
	glDeleteShader(vertexShaderObject);
	glDeleteShader(fragmentShaderObject);
}

int Shader::getUniformLocation(const std::string& uniformName) const
{
	if (!m_uniformLocationMap.contains(uniformName))
	{
		m_uniformLocationMap[uniformName] = glGetUniformLocation(m_program, uniformName.c_str());
	}
	return m_uniformLocationMap.at(uniformName);
}
