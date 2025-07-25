#include <iostream>
#include <fstream>
#include <glad/glad.h>
#include "shader.hpp"

Shader::Shader(){
	this->program = glCreateProgram();
	
}

std::string Shader::readFile(const std::string filepath){
	std::fstream fs(filepath);
	std::string sout;
	std::string s;

	if (!fs.is_open()){
		std::cerr << "Failed to open file: " << filepath << std::endl;
		return "";
        }

	while (std::getline(fs, s)){
		sout += s;
		sout.push_back('\n');
        }
	sout.pop_back();
	sout.push_back('\0');
	return sout;
}

int Shader::addShader(const std::string filepath, GLenum shaderType){
	std::string source = this->readFile(filepath);
	
	if (source == "" ) {
		std::cerr << "Source File Empty" << std::endl;
		return -1;
	}
	const char * csource = source.c_str();
	GLuint newshader = glCreateShader(shaderType);
	glShaderSource(newshader,1,&csource,nullptr);
	glCompileShader(newshader);

	int success;
	char infolog[512];
	glGetShaderiv(newshader, GL_COMPILE_STATUS, &success);
	if (!success){
		glGetProgramInfoLog(newshader, 512, nullptr, infolog);
		std::cerr << filepath << "\n";
		std::cerr << "Shader Compilation Faliure: " << infolog << std::endl;
		return -1;
	}
	this->filepaths.push_back(filepath);
	
	glAttachShader(this->program, newshader);
	return 0;
}

void Shader::useProgram(){
	glUseProgram(this->program);
}

int Shader::link(){
	int success;
	char infolog[512];

	glLinkProgram(this->program);
	glGetProgramiv(this->program, GL_LINK_STATUS, &success);
	if (!success){
		glGetProgramInfoLog(this->program, 512, nullptr, infolog);
		std::cerr << "Shader Linking Faliure: " << infolog << std::endl;
		return -1;
	}
	return 0;
}

