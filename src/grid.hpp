

#ifndef GRID_HPP
#define GRID_HPP

#include <glm/glm.hpp>
#include <vector>

class Grid{
	public:
		uint width;
		std::vector<glm::vec4> vertices;

		Grid(uint width);
};








#endif //GRID_HPP
