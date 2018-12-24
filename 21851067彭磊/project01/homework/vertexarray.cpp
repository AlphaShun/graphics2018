#include "vertexarray.h"
#include "object.h"
VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_ArrayID);                 //����VAO
}
VertexArray::~VertexArray() 
{
	for (int i = 0; i < m_Buffers.size(); i++)
		delete m_Buffers[i];                          //ɾ�������е�vboָ��ָ�������
	glDeleteVertexArrays(1, &m_ArrayID);              //ɾ��vao
}
void VertexArray::bind() const                         //��װbind
{
	glBindVertexArray(m_ArrayID);   //��VAO����
}

void VertexArray::unbind() const                     //��װ���
{
	glBindVertexArray(0);
}
void VertexArray::addBuffer(Buffer *buffer, GLuint index, GLuint stride, const void* offset) {
	bind();
	buffer->bind();

	glEnableVertexAttribArray(index);                                                       //�������� ��Ӧ����vert���layout
	glVertexAttribPointer(index, buffer->getComponentCount(), GL_FLOAT, GL_FALSE, stride, offset);    //���Ͷ������ã�������������ʽ
	m_Buffers.push_back(buffer);                                                            //�Ѷ���ѹ������

	buffer->unbind();                                                                       //�Ƚ��vbo
	unbind();                                                                               //�ٽ��vao
}