#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_map>


class Shader
{
public:
	Shader(const std::string& path);
	~Shader();
	void bind() const;
	void unbind() const;
	unsigned int getID() const;
	void setBool(const std::string& name, const std::vector<bool>& value) const;
	void setInt(const std::string& name, const std::vector<int>& value) const;
	void setFloat(const std::string& name, const std::vector<float>& value) const;

	void setVec(const std::string& name, const std::vector<float>& value) const;
	void setVec(const std::string& name, const glm::vec2& value) const;
	void setVec(const std::string& name, const glm::vec3& value) const;
	void setVec(const std::string& name, const glm::vec4& value) const;
	
	void setMat(const std::string& name, const glm::mat2& mat) const;
	void setMat(const std::string& name, const glm::mat3& mat) const;
	void setMat(const std::string& name, const glm::mat4& mat) const;

private:
	unsigned int m_ID;
	std::string m_filePath;
	std::unordered_map<std::string, int> m_UniformLocationCache;

	unsigned int compileShader(unsigned int shaderType, const char* shaderSource);
	void parseSource(const std::string& path, std::string& o_vs, std::string& o_fs);
	int getUniformLocation(const std::string& name);
};

