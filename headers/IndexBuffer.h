#pragma once
class IndexBuffer
{
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void bind() const;
	void unbind() const;
	unsigned int getCount() const;
private:
	unsigned int m_ID;
	unsigned int m_count;
};

