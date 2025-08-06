#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <string>
#include "camera.hpp"
#include "shader.hpp"




Camera::Camera(glm::vec3 pos, glm::vec3 look_at, glm::vec3 up,
	       float fov, float aspect_ratio, 
	       float near_clip, float far_clip) : 
	       position(pos), looking_at(look_at),
       	       world_up(up), fov(fov), aspect_ratio(aspect_ratio),
	       near_clip(near_clip), far_clip(far_clip){};

void Camera::setPVmatrixUniform(Shader shader, std::string uniformName){
	glm::mat4 viewMatrix = glm::lookAt(this->position,this->looking_at,this->world_up);
	glm::mat4 projectionMatrix = glm::perspective(this->fov,this->aspect_ratio,this->near_clip,this->far_clip);
	
	glm::mat4 pvMatrix = projectionMatrix * viewMatrix;
	shader.useProgram();
	GLuint location = glGetUniformLocation(shader.getProgram(),uniformName.c_str());
	glUniformMatrix4fv(location,1,GL_FALSE, glm::value_ptr(pvMatrix));
}
