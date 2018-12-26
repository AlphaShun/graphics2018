#include "buffer.h"
Buffer::Buffer(GLfloat *data, GLsizei count, GLuint componentCount) :m_ComponentCount(componentCount)
{
	glGenBuffers(1, &m_BufferID);													  //�������㻺�����
	glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);										  //�󶨵�GL_ARRAY_BUFFER��
	glBufferData(GL_ARRAY_BUFFER, count, data, GL_STATIC_DRAW);//��vertices�е����ݸ��Ƶ���ǰ�󶨵Ļ�����
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
Buffer::~Buffer() 
{
	glDeleteBuffers(1, &m_BufferID);                                       
}
void Buffer::bind() const 
{
	glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);                                 //��װ��
}
void Buffer::unbind() const 
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);                                          //��װ���
}
