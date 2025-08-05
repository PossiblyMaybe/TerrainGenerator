

#ifndef SHADER_HPP
#define SHADER_HPP
#include <glad/glad.h>
#include <vector>
#include <string>

class Shader{
private:
	GLuint program{};
	std::vector<std::string> filepaths{};

	std::string readFile(const std::string filepath);
public:
	int addShader(const std::string filepath, GLenum shaderType);
	int link();	
	Shader();
	void useProgram();
	GLuint getProgram();
};




#endif //SHADER_HPP
