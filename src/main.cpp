#include <execinfo.h>
#include <cstdio>
#include <csignal>

#include <string>
#include <array>
#include <iostream>
#include <fstream>
#include <algorithm>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"
#include "camera.hpp"
#include "grid.hpp"
#include "generator.hpp"


#define TEXTURE_WIDTH 1024u


void handler(int sig){
	void *array[10];
	size_t size;
	size = backtrace(array,10);

	std::cerr << "Error: " << sig << "\n";
	backtrace_symbols_fd(array,size, STDERR_FILENO);
	exit(1);
}

void ErrCallbackGLFW(int, const char * err_str){
	std::cerr << "GLFW_ERR: " << err_str << std::endl;
}

void framebufferSizeCallback(GLFWwindow * window, int width, int height){
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window){
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
		glfwSetWindowShouldClose(window, true);
	}
}




int main(){

	//ERROR HANDLING INCASE I BREAK SHIT

	signal(SIGABRT, handler);
	signal(SIGSEGV, handler);


	//GLFW AND GLAD INIT

	glfwInit();
	glfwSetErrorCallback(ErrCallbackGLFW);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800,600,"TerrainGen",nullptr,nullptr);

	if (window == nullptr){
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		std::cout << "Failed to init GLAD" << std::endl;
		return -1;
	}


	glViewport(0,0,800,600);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	framebufferSizeCallback(window,800,600);



	// SHADERS

	Shader outShader{};
	outShader.addShader("src/shaders/vertex.vert",GL_VERTEX_SHADER);
	outShader.addShader("src/shaders/fragment.frag",GL_FRAGMENT_SHADER);
	outShader.link();


	Shader noiseShader{};
	noiseShader.addShader("src/shaders/noise2.comp", GL_COMPUTE_SHADER);
        noiseShader.link();

	// GENERATOR

	Grid terrainGrid(TEXTURE_WIDTH);
	Generator terrainGen(noiseShader, terrainGrid, glm::vec2(15.2345,24.4),
		             "src/shaders/hash.comp", "src/shaders/simplexPreComp.comp",
			     "src/shaders/normal.comp");
	
	terrainGen.addIteration(waveData{10.0f,1024u});
	terrainGen.addIteration(waveData{0.7f,512u});
	terrainGen.addIteration(waveData{0.6f,256u});
	terrainGen.addIteration(waveData{40.0f,128u});
	terrainGen.addIteration(waveData{0.5f,64u});
	terrainGen.addIteration(waveData{0.0f,32u});
	terrainGen.addIteration(waveData{0.0f,16u});
	terrainGen.addIteration(waveData{0.0f,8u});
	terrainGen.addIteration(waveData{0.0f,4u});
	
	
	// UNIFORMS
	
	glBindVertexArray(terrainGen.getVAO());
	glEnableVertexAttribArray(0);

	glm::mat4 viewMatrix = glm::lookAt(glm::vec3(-100,300,-100), glm::vec3(512,0,512),glm::vec3(0,1,0));
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(90.0f),4.0f/3.0f,0.1f,3000.0f);

	outShader.useProgram();
	GLuint viewLoc = glGetUniformLocation(outShader.getProgram(),"view");
	GLuint projLoc = glGetUniformLocation(outShader.getProgram(),"projection");

	glUniformMatrix4fv(viewLoc,1,GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(projLoc,1,GL_FALSE, glm::value_ptr(projectionMatrix));

	GLuint lightLoc = glGetUniformLocation(outShader.getProgram(),"lightColour");
	GLuint lightPosLoc = glGetUniformLocation(outShader.getProgram(),"lightPos");
	glUniform3f(lightLoc,1.0f,1.0f,1.0f);
	glUniform3f(lightPosLoc,1024.0f,1024.0f,0.0f);
	
	
	//Get heights
	terrainGen.runHashCalc();
	terrainGen.run();
	
	//PGM heightmap
	terrainGen.writeToPGM();

	//LOOP

	while (!glfwWindowShouldClose(window)){
		processInput(window);

		glClearColor(0.0f,0.0f,0.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);	

		outShader.useProgram();
		terrainGen.bindGridEBO();
		glDrawElements(GL_TRIANGLES,terrainGrid.getIndexCount(), GL_UNSIGNED_INT, 0);
		
		glfwSwapBuffers(window);
      
		glfwPollEvents();
	}
	
	glfwTerminate();
	return 0;
}
