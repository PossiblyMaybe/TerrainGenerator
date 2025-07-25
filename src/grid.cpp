
#include <glm/glm.hpp>
#include <vector>
#include <cmath>

#include "grid.hpp"

Grid::Grid(uint width) : width(width) {
	this->vertices = std::vector<glm::vec4>(pow(this->width,2));
	int index = 0;
	for (int i=0;i<this->width;++i){
		for (int j=0;j<this->width;++j){
			this->vertices[index] = glm::vec4(j,i,0,0);
			++index;	
		}
	}
}
