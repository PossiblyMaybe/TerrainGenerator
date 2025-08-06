

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <cmath>
#include <string>
#include "shader.hpp"

class Camera{
	private:
		glm::vec3 position, looking_at, world_up, camera_up;
		float fov, aspect_ratio, near_clip, far_clip;
	public:
		Camera(glm::vec3 pos, glm::vec3 look_at, glm::vec3 up,
		       float fov=M_PI_2f, float aspect_ratio=1.33f,
		       float near_clip=0.1f, float far_clip=2000.0f);
		void setPVmatrixUniform(Shader shader, std::string uniformName);
};


#endif //CAMERA_HPP
