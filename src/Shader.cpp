
#include "../headers/util.h"
#include "../headers/Shader.h"

Shader::Shader(const std::string& path) 
	: m_filePath(path)
{
	std::string vertexCode;
	std::string fragmentCode;
	parseSource(path, vertexCode, fragmentCode);
	
	unsigned int vShader = compileShader(GL_VERTEX_SHADER, vertexCode.c_str());
	unsigned int fShader = compileShader(GL_FRAGMENT_SHADER, fragmentCode.c_str());
	
	GLCall(m_ID = glCreateProgram());
	GLCall(glAttachShader(m_ID, vShader));
	GLCall(glAttachShader(m_ID, fShader));
	GLCall(glLinkProgram(m_ID));

	int success;
	char infoLog[512];
	GLCall(glGetProgramiv(m_ID, GL_LINK_STATUS, &success));
	if (!success) {
		GLCall(glGetProgramInfoLog(m_ID, 512, 0, infoLog));
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILIED\n" << infoLog << std::endl;
	}

	GLCall(glDeleteShader(vShader));
	GLCall(glDeleteShader(fShader));
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_ID));
}

void Shader::bind() const {
	GLCall(glUseProgram(m_ID));
}

void Shader::unbind() const
{
	GLCall(glUseProgram(0));
}

unsigned int Shader::getID() const
{
	return m_ID;
}

void Shader::setBool(const std::string& name, const std::vector<bool>& value) const {
	size_t count = value.size();
	switch (count)
	{
	case 1: {
		GLCall(glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (int)value[0]));
		break;
	}
	case 2: {
		GLCall(glUniform2i(glGetUniformLocation(m_ID, name.c_str()), (int)value[0], (int)value[1]));
		break;
	}
	case 3: {
		GLCall(glUniform3i(glGetUniformLocation(m_ID, name.c_str()), (int)value[0], (int)value[1], (int)value[2]));
		break;
	}
	case 4: {
		GLCall(glUniform4i(glGetUniformLocation(m_ID, name.c_str()), (int)value[0], (int)value[1], (int)value[2], (int)value[3]));
		break;
	}
	default: {
		std::cout << "Invalid Uniform type size, not in range [1,4] for uniform " << name << std::endl;
		break;
	}
	}
}

void Shader::setInt(const std::string& name, const std::vector<int>& value) const {
	size_t count = value.size();
	switch (count)
	{
	case 1: {
		GLCall(glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value[0]));
		break;
	}
	case 2: {
		GLCall(glUniform2i(glGetUniformLocation(m_ID, name.c_str()), value[0], value[1]));
		break;
	}
	case 3: {
		GLCall(glUniform3i(glGetUniformLocation(m_ID, name.c_str()), value[0], value[1], value[2]));
		break;
	}
	case 4: {
		GLCall(glUniform4i(glGetUniformLocation(m_ID, name.c_str()), value[0], value[1], value[2], value[3]));
		break;
	}
	default: {
		std::cout << "Invalid Uniform type size, not in range [1,4] for uniform " << name << std::endl;
		break;
	}
	}
}

void Shader::setFloat(const std::string& name, const std::vector<float>& value) const {
	size_t count = value.size();
	switch (count)
	{
	case 1: {
		GLCall(glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value[0]));
		break;
	}
	case 2: {
		GLCall(glUniform2f(glGetUniformLocation(m_ID, name.c_str()), value[0], value[1]));
		break;
	}
	case 3: {
		GLCall(glUniform3f(glGetUniformLocation(m_ID, name.c_str()), value[0], value[1], value[2]));
		break;
	}
	case 4: {
		GLCall(glUniform4f(glGetUniformLocation(m_ID, name.c_str()), value[0], value[1], value[2], value[3]));
		break;
	}
	default: {
		std::cout << "Invalid Uniform type size, not in range [1,4] for uniform " << name << std::endl;
		break;
	}
	}
}

void Shader::setVec(const std::string& name, const std::vector<float>& value) const {
	Shader::setFloat(name, value);
}

void Shader::setVec(const std::string& name, const glm::vec2& value) const
{
	GLCall(glUniform2fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]));
}

void Shader::setVec(const std::string& name, const glm::vec3& value) const
{
	GLCall(glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]));
}

void Shader::setVec(const std::string& name, const glm::vec4& value) const
{
	GLCall(glUniform4fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]));
}

void Shader::setMat(const std::string& name, const glm::mat2& mat) const
{
	GLCall(glUniformMatrix2fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]));
}

void Shader::setMat(const std::string& name, const glm::mat3& mat) const
{
	GLCall(glUniformMatrix3fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]));
}

void Shader::setMat(const std::string& name, const glm::mat4& mat) const
{
	GLCall(glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]));
}

unsigned int Shader::compileShader(unsigned int shaderType, const char* shaderSource) {
	GLCall(unsigned int shader = glCreateShader(shaderType));
	GLCall(glShaderSource(shader, 1, &shaderSource, 0));
	GLCall(glCompileShader(shader));

	int success;
	char infoLog[512];
	GLCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
	if (!success) {
		GLCall(glGetShaderInfoLog(shader, 512, 0, infoLog));
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	return shader;
}

void Shader::parseSource(const std::string& path, std::string& o_vs, std::string& o_fs) {
	std::ifstream shaderFile(path);
	if (!shaderFile) {
		std::cout << "ERROR::SHADER::INvalueID_SOURCE_PATH\n";
		return;
	}

	enum class shaderType {
		NONE = -1, VERTEX, FRAGMENT
	};

	shaderType type = shaderType::NONE;
	std::stringstream sstream[2];
	std::string line;
	while (getline(shaderFile, line)) {
		if (line.find("#SHADER") != std::string::npos) {
			if (line.find("VERTEX") != std::string::npos) {
				type = shaderType::VERTEX;
			}
			else if (line.find("FRAGMENT") != std::string::npos) {
				type = shaderType::FRAGMENT;
			}
		}
		else if (type != shaderType::NONE) {
			sstream[(unsigned int)type] << line << '\n';
		}
	}
	o_vs = sstream[0].str();
	o_fs = sstream[1].str();
}

int Shader::getUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.count(name)) return m_UniformLocationCache[name];

	GLCall(int location = glGetUniformLocation(m_ID, name.c_str()));
	if (location == -1) {
		std::cout << "Warning: Uniform " << name << " doesn't exist." << std::endl;
	}

	return m_UniformLocationCache[name] = location;
}
