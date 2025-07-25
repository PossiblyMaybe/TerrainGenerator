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

#include "shader.hpp"
#include "camera.hpp"
#include "grid.hpp"
#include "generator.hpp"


#define TEXTURE_WIDTH 1024u

void ErrCallbackGLFW(int, const char * err_str){
	std::cout << "GLFW_ERR: " << err_str << std::endl;
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
	//init

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


	//IndexArray
	
	std::vector<std::array<int, 3>> indices(TEXTURE_WIDTH*TEXTURE_WIDTH);
	int toprow = TEXTURE_WIDTH*TEXTURE_WIDTH - TEXTURE_WIDTH;
	int bottomrow = TEXTURE_WIDTH;
	std::array<int, 3> triIndex{0,0,0};
	int index = 0;
	for(int i=0;i<TEXTURE_WIDTH*TEXTURE_WIDTH;i+=2){
		triIndex[0] = i;
		if (i < toprow){
			triIndex[1] = i+1;
			triIndex[2] = i+TEXTURE_WIDTH+1;
			indices[index] = triIndex;
			++index;
		} 
		if (i > bottomrow){
			triIndex[2] = i+1;
			triIndex[1] = i-TEXTURE_WIDTH;
			indices[index] = triIndex;
			++index;
		}
	}

	Shader outShader{};
	outShader.addShader("src/shaders/vertex.vert",GL_VERTEX_SHADER);
	outShader.addShader("src/shaders/fragment.frag",GL_FRAGMENT_SHADER);
	outShader.link();


	Shader noiseShader{};
	noiseShader.addShader("src/shaders/noise2.comp", GL_COMPUTE_SHADER);
        noiseShader.link();

	// GENERATOR

	Grid terrainGrid(TEXTURE_WIDTH);
	Generator terrainGen(noiseShader, "src/shaders/hash.comp", "src/shaders/simplexPreComp.comp",terrainGrid, glm::vec2(15.2345,24.4));
	terrainGen.addIteration(waveData{1.0f,1024u});
	terrainGen.addIteration(waveData{0.7f,512u});
	terrainGen.addIteration(waveData{0.6f,256u});
	terrainGen.addIteration(waveData{1.0f,128u});
	terrainGen.addIteration(waveData{0.5f,64u});
	terrainGen.addIteration(waveData{0.0f,32u});
	terrainGen.addIteration(waveData{0.0f,16u});
	terrainGen.addIteration(waveData{0.0f,8u});
	terrainGen.addIteration(waveData{0.0f,4u});
	

	//EBO	
		
	GLuint EBO;
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(indices[0]), indices.data(),GL_DYNAMIC_DRAW);	

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	// CAMERA	

	Camera cam(glm::vec3(500.0f,300.0f,300.0f),
		   glm::vec3(0.0f,1.0f,0.0f),-0.3f,0.0f,0.0f,
		   0.1,1.0);	
	glm::mat4 view = cam.createViewMatrix();		
	glm::mat4 projectionMatrix = glm::perspective(M_PIf, 800.0f/600.0f, 0.1f, 100.0f);
	terrainGen.runHashCalc();
	terrainGen.run();
	terrainGen.writeToPGM();

	//LOOP

	while (!glfwWindowShouldClose(window)){
		processInput(window);

		glClearColor(0.0f,0.0f,0.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);	
	
		outShader.useProgram();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES,indices.size(), GL_UNSIGNED_INT, 0);
	
		glfwSwapBuffers(window);
      
		glfwPollEvents();
	}
	
	glfwTerminate();
	return 0;
}
