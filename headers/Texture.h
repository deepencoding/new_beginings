#pragma once

#include <string>

class Texture
{
public:
	Texture(const std::string& path);
	~Texture();

	void bind(unsigned int slot = 0) const;
	void unbind() const;
	int getWidth() const;
	int getHeight() const;

private:
	unsigned int m_ID;
	std::string m_filePath;
	unsigned char* m_localBuffer;
	int m_width, m_height, m_BPP;
};

