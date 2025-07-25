


#ifndef GENERATOR_HPP
#define GENERATOR_HPP
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include "shader.hpp"
#include "grid.hpp"

#define WAVEDATA_SIZE 12

struct waveData {
	float amplitude;
	uint wavelength;
};

enum WaveDataType : uint {
	WAVE_AMPLITUDE,
	WAVE_WAVELENGTH
};

union waveDataUnion{
	float amplitude;
	uint wavelength;
};

class Generator{
	private:
		Grid grid;
		glm::vec2 seed;
		std::vector<waveData> iterationInfo{};
	public:
		GLuint gridSSBO, simplexSSBO, infoUBO, seedUBO;
		Shader noise, simplex, hash;
		//public:
		Generator(Shader noise,std::string hashPath, std::string simplexPath, Grid grid, glm::vec2 seed);
		void addIteration(waveData newinfo);
		void removeIteration(uint index);
		int editIteration(uint index, WaveDataType t, waveDataUnion newData);
		void run();
		void runHashCalc();
		void setSeed(glm::vec2 seed);
		GLuint getGridSSBO();
		void writeToPGM();
	
};






#endif //GENERATOR_HPP
