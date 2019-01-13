#include "vertexarray.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_ArrayID);                 //����VAO
}
VertexArray::~VertexArray() 
{
	for (int i = 0; i < m_Buffers.size(); i++)
		delete m_Buffers[i];                          //ɾ�������е�vboָ��ָ�������
	for (int i = 0; i < m_Textures.size(); i++)
		delete m_Textures[i];
	glDeleteVertexArrays(1, &m_ArrayID);              //ɾ��vao
}
void VertexArray::bind() const                         //��װbind
{
	for (auto it = m_Textures.begin(); it != m_Textures.end(); it++)
	{
		(*it)->aBind();
	}
	glBindVertexArray(m_ArrayID);   //��VAO����
}

void VertexArray::unbind() const                     //��װ���
{
	glBindVertexArray(0);
}
void VertexArray::addBuffer(Buffer *buffer, GLuint index, GLuint stride, GLuint offset) {
	bind();
	buffer->bind();

	                                                     //�������� ��Ӧ����vert���layout
	glVertexAttribPointer(index, buffer->getComponentCount(), GL_FLOAT, GL_FALSE, stride* sizeof(GLfloat), (void*)(offset*sizeof(GLfloat)));    //���Ͷ������ã�������������ʽ
	glEnableVertexAttribArray(index);
	m_Buffers.push_back(buffer);                                                            //�Ѷ���ѹ������

	                                                                    //�Ƚ��vbo
	                                                                              //�ٽ��vao
}
void VertexArray::addTexture(Texture *texture, GLuint index, GLuint stride, GLuint offset)
{
	bind();
	texture->bind();
	//�������� ��Ӧ����vert���layout

	glVertexAttribPointer(index, texture->getComponentCount(), GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (void*)(offset * sizeof(GLfloat)));
	glEnableVertexAttribArray(index);
	m_Textures.push_back(texture);
	
	
	
}