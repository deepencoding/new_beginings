#include <iostream>

#include "../headers/util.h"
#include "../headers/Texture.h"
#include "vendor/stb_image/stb_image.h"

Texture::Texture(const std::string& path) 
	: m_ID(0), m_filePath(path), m_localBuffer(nullptr), m_width(0), m_height(0), m_BPP(0)
{
	stbi_set_flip_vertically_on_load(true);
	unsigned char* m_localBuffer = stbi_load(path.c_str(), &m_width, &m_height, &m_BPP, 4);
	
	if (!m_localBuffer) {
		std::cout << "ERROR::TEXTURE::FAILED_TO_LOAD_TEXTURE\n" << std::endl;
		std::cout << stbi_failure_reason() << std::endl;
		__debugbreak();
		return;
	}

	GLCall(glGenTextures(1, &m_ID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_ID));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer));
	GLCall(glGenerateMipmap(GL_TEXTURE_2D));

	if (m_localBuffer) stbi_image_free(m_localBuffer);
}

Texture::~Texture() {
	GLCall(glDeleteTextures(1, &m_ID));
}

void Texture::bind(unsigned int slot) const {
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_ID));
}
void Texture::unbind() const {
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
int Texture::getWidth() const {
	return m_width;
}
int Texture::getHeight() const {
	return m_height;
}