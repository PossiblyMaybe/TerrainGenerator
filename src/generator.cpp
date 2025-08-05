#include <glad/glad.h>

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

#include "generator.hpp"


Generator::Generator(Shader noise,Grid grid, glm::vec2 seed,
	             std::string hashPath, std::string simplexPath,
		     std::string normalPath) : noise(noise), grid(grid), seed(seed) {

	this->simplex = Shader();
	this->hash = Shader();
	this->normal = Shader();
	this->hash.addShader(hashPath, GL_COMPUTE_SHADER);
	this->hash.link();
	this->simplex.addShader(simplexPath, GL_COMPUTE_SHADER);
	this->simplex.link();
	this->normal.addShader(normalPath, GL_COMPUTE_SHADER);
	this->normal.link();

	glGenVertexArrays(1,&this->VAO);

	glGenBuffers(1,&this->gridSSBO);
	glGenBuffers(1,&this->simplexSSBO);
	glGenBuffers(1,&this->infoUBO);
	glGenBuffers(1,&this->seedUBO);
	glGenBuffers(1,&this->gridEBO);
	glGenBuffers(1,&this->normalSSBO);

	glBindVertexArray(VAO);

	// gridSSBO
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->gridSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, this->grid.vertices.size()*sizeof(glm::vec4),&this->grid.vertices[0],GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, gridSSBO);

	glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,4*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	// simplexSSBO
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->simplexSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec2)*grid.vertices.size(), nullptr, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, this->simplexSSBO);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	// normalSSBO
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->normalSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, this->grid.vertices.size()*sizeof(glm::vec3),nullptr,GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, this->normalSSBO);

	// seedUBO
	glBindBuffer(GL_UNIFORM_BUFFER, this->seedUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec2), &this->seed, GL_STATIC_READ);
	glBindBufferBase(GL_UNIFORM_BUFFER, 2,this->seedUBO);

	// infoUBO
	glBindBuffer(GL_UNIFORM_BUFFER, this->infoUBO);
	glBindBufferBase(GL_UNIFORM_BUFFER, 3, this->infoUBO);

	glBindBuffer(GL_UNIFORM_BUFFER,0);

	// indices EBO
	
	std::vector<std::array<uint,3>> indices = this->grid.getTriIndices();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->gridEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(uint)*3, indices.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

	glBindVertexArray(0);

	//simplex pre-compute
	simplex.useProgram();
	glDispatchCompute(this->grid.width,this->grid.width,1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void Generator::runHashCalc(){
	hash.useProgram();
	glDispatchCompute(this->grid.width, this->grid.width ,1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);	
}

void Generator::addIteration(waveData newinfo){
	this->iterationInfo.push_back(newinfo);
}

void Generator::removeIteration(uint index){
	this->iterationInfo.erase(this->iterationInfo.begin() + index);
}

int Generator::editIteration(uint index, WaveDataType t, waveDataUnion newData){
	switch (t){
		case (WAVE_AMPLITUDE):
			this->iterationInfo[index].amplitude = newData.amplitude;
			break;
		case (WAVE_WAVELENGTH):
			this->iterationInfo[index].wavelength = newData.wavelength;
			break;
	}
	return 0;
}

void Generator::run(){
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT); //Just to make sure no other compute shaders are running when this starts

	this->noise.useProgram();
	
	for (int i=0;i<this->iterationInfo.size();++i){
		glBindBuffer(GL_UNIFORM_BUFFER,this->infoUBO);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(this->iterationInfo[i]), &(*(this->iterationInfo.begin()+i)), GL_DYNAMIC_READ);
		glBindBuffer(GL_UNIFORM_BUFFER,0);

		glDispatchCompute(this->grid.width,this->grid.width,1);	
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);	
	}
	this->normal.useProgram();
	glDispatchCompute(this->grid.width,this->grid.width,1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void Generator::setSeed(glm::vec2 seed){
	this->seed = seed;
}


GLuint Generator::getGridSSBO(){
	return this->gridSSBO;
}



void Generator::writeToPGM(){
	std::ofstream outfile;
	outfile.open("heightmap.pgm");

	std::vector<glm::vec4> vertexdata(this->grid.vertices.size());
	glGetNamedBufferSubData(this->gridSSBO,0,vertexdata.size()*sizeof(glm::vec4), &vertexdata[0]);
	
	outfile << "P2\n1024 1024\n65535\n";
	std::vector<float> heights(this->grid.vertices.size());
	for (int i=0;i<this->grid.vertices.size();++i){
		heights[i] = vertexdata[i][2];
	}

	float maxheight = *std::max_element(heights.begin(),heights.end());
	
	for (int i=0;i<this->grid.vertices.size();++i){
		outfile << int(65535.0f*(heights[i]/maxheight)) << " ";
	}
	outfile.close();	
}

GLuint Generator::getVAO(){
	return this->VAO;
}

void Generator::bindGridEBO(){
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->gridEBO);
}

