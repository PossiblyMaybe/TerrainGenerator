
#include <glm/glm.hpp>
#include <vector>
#include <array>
#include <cmath>

#include "grid.hpp"

Grid::Grid(uint width) : width(width) {
	this->vertices = std::vector<glm::vec4>(pow(this->width,2));
	this->indexCount = this->vertices.size()*6;
	int index = 0;
	for (int i=0;i<this->width;++i){
		for (int j=0;j<this->width;++j){
			this->vertices[index] = glm::vec4(j,i,0,0);
			++index;	
		}
	}
}


std::vector<std::array<uint, 3>> Grid::getTriIndices(){
	std::vector<std::array<uint, 3>> outvec(this->vertices.size()*2);
	uint index=0;
	uint toprow = this->vertices.size() - this->width;
	for (uint i=0;i<this->vertices.size();++i){
		if ((i+1)%this->width != 0 && i<toprow){
			outvec[index] = {i,i+1,i+this->width};
		}
		if (i%this->width != 0 && i>this->width){
			outvec[++index] = {i, i-this->width, i-1};
		}
		index++;
	}
	return outvec;
}

uint Grid::getIndexCount(){
	return this->indexCount;
}
