#include "shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath) 
	: m_VertPath(vertexPath), m_FragPath(fragmentPath) {

	m_ShaderID = createShader();
}

Shader::~Shader() {
	glDeleteProgram(m_ShaderID);
}

GLuint Shader::createShader() {
	GLuint program = glCreateProgram();
	GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);

	std::string vertSourceString = FileUtils::read_file(m_VertPath);
	std::string fragSourceString = FileUtils::read_file(m_FragPath);

	const char *vertSource = vertSourceString.c_str();
	const char *fragSource = fragSourceString.c_str();


	glShaderSource(vertex, 1, &vertSource, NULL);
	glCompileShader(vertex);
	GLint result;
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		GLint length;
		glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &length);
		std::vector<char> error(length);
		glGetShaderInfoLog(vertex, length, &length, &error[0]);
		std::cout << "Fail to Compile Vertex Shader!" << std::endl << &error[0] << std::endl;
		glDeleteShader(vertex);
		return 0;
	}
	checkCompileErrors(vertex, "VERTEX");//

	glShaderSource(fragment, 1, &fragSource, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		GLint length;
		glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &length);
		std::vector<char> error(length);
		glGetShaderInfoLog(fragment, length, &length, &error[0]);
		std::cout << "Fail to Compile Fragment Shader!" << std::endl << &error[0] << std::endl;
		glDeleteShader(fragment);
		return 0;
	}
	checkCompileErrors(fragment, "FRAGMENT");//

	glAttachShader(program, vertex);
	glAttachShader(program, fragment);

	glLinkProgram(program);
	checkCompileErrors(program, "PROGRAM");//
	glValidateProgram(program);

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return program;
}

void Shader::checkCompileErrors(unsigned int ID, std::string type) {
	int success;
	char infoLog[512];

	if (type != "PROGRAM")
	{
		glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(ID, 512, NULL, infoLog);
			std::cout << "shader compile error " << infoLog << std::endl;
		}
	}
	else
	{
		glGetShaderiv(ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(ID, 512, NULL, infoLog);
			std::cout << "shader link error " << infoLog << std::endl;
		}
	}
}

//GLint Shader::getUniformLocation(const GLchar *name) {
//	return glGetUniformLocation(m_ShaderID, name);
//}
//
//void Shader::setUniform1f(const GLchar * name, float value) {
//	glUniform1f(getUniformLocation(name), value);
//}
//void Shader::setUniform1i(const GLchar * name, int value) {
//	glUniform1i(getUniformLocation(name), value);
//}
//void Shader::setUniform2f(const GLchar * name, const vec2 &v) {
//	glUniform2f(getUniformLocation(name),v.x,v.y);
//}
//void Shader::setUniform3f(const GLchar * name, const vec3 &v) {
//	glUniform3f(getUniformLocation(name), v.x, v.y, v.z);
//}
//void Shader::setUniform4f(const GLchar * name, const vec4 &v) {
//	glUniform4f(getUniformLocation(name), v.x, v.y, v.z, v.w);
//}
//void Shader::setUniformMat4(const GLchar * name, const mat4 &matrix) {
//	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, matrix.elements);
//}

void Shader::enable() const {
	glUseProgram(m_ShaderID);
}

void Shader::disable() const{
	glUseProgram(0);
}

GLuint Shader::getID() {
	return m_ShaderID;
}

