#pragma once


#include <vector>
#include <GL/glew.h>

#include "buffer.h"
#include "texturebuffer.h"

class VertexArray {
private:
	GLuint m_ArrayID;
	std::vector<Buffer*> m_Buffers;      //ʢװָ��bufferָ�������
	std::vector<Texture*> m_Textures;

public:
	VertexArray();
	~VertexArray();
	void addBuffer(Buffer *buffer, GLuint index, GLuint stride, GLuint offset);
	void addTexture(Texture *texture, GLuint index, GLuint stride, GLuint offset);
	void bind() const;
	void unbind() const;
};