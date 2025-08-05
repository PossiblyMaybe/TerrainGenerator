

#ifndef GRID_HPP
#define GRID_HPP

#include <glm/glm.hpp>
#include <vector>
#include <array>

class Grid{
	private:
		uint indexCount;
	public:
		uint width;
		std::vector<glm::vec4> vertices;

		Grid(uint width);
		std::vector<std::array<uint, 3>> getTriIndices();
		uint getIndexCount();
};








#endif //GRID_HPP
