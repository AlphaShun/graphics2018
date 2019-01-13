#pragma once
#include"shader.h"

Shader::Shader(const char *vertexPath, const char * fragmentPath) :m_VertPath(vertexPath), m_FragPath(fragmentPath)
{
	m_ShaderID = load();
}

GLuint Shader::load()
{
	GLuint program = glCreateProgram();
	GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
	std::string vertSourceString = FileUtils::read_file(m_VertPath);
	std::string fragSourceString = FileUtils::read_file(m_FragPath); //��ȡ��ɫ��Դ�ļ�����

	const char *vertSource = vertSourceString.c_str();
	const char *fragSource = fragSourceString.c_str();

	/*������ɫ��*/
	glShaderSource(vertex, 1, &vertSource, NULL);   //������ɫ��Դ�������ɫ������
	glCompileShader(vertex); //���붥����ɫ��

	/*������ɹ����*/
	GLint result;
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		GLint length;
		glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &length);
		char *infoLog = new char[length + 1];
		glGetShaderInfoLog(vertex, length, &length, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		glDeleteShader(vertex);
		delete[]infoLog;
		return 0;
	}

	/*Ƭ����ɫ��*/
	glShaderSource(fragment, 1, &fragSource, NULL);   //Ƭ����ɫ��Դ�������ɫ������
	glCompileShader(fragment); //����Ƭ����ɫ��

	/*������ɹ����*/

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		GLint length;
		glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &length);
		char *infoLog = new char[length + 1];
		glGetShaderInfoLog(fragment, length, &length, infoLog);
		std::cout << "ERROR::SHADER::FRAG::COMPILATION_FAILED\n" << infoLog << std::endl;
		glDeleteShader(fragment);
		delete[]infoLog;
		return 0;
	}
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);

	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return program;
}
void Shader::enble()const
{
	glUseProgram(m_ShaderID);
}

GLuint Shader::getID() const
{
	return m_ShaderID;
}
void Shader::disable() const
{
	glUseProgram(0);
}
Shader::~Shader()
{
}
void Shader::setUniform1f(const GLchar *name, float value)
{
	glUniform1f(getUniformLocation(name), value);
}
void Shader::setUniform1i(const GLchar *name, int value)
{
	glUniform1i(getUniformLocation(name), value);
}
void Shader::setUniform2f(const GLchar *name, const vec2 &v)
{
	glUniform2f(getUniformLocation(name), v.getX(), v.getY());
}
void Shader::setUniform3f(const GLchar *name, const vec3 &v)
{
	glUniform3f(getUniformLocation(name), v.getX(), v.getY(), v.getZ());
}
void Shader::setUniform4f(const GLchar *name, const vec4 &v)
{
	glUniform4f(getUniformLocation(name), v.getX(), v.getY(), v.getZ(), v.getW());
}
void Shader::setUniformMat4(const GLchar *name, const mat4 &mat)
{
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, mat.element);
}
GLuint Shader::getUniformLocation(const GLchar *name)
{
	return glGetUniformLocation(m_ShaderID, name);
}