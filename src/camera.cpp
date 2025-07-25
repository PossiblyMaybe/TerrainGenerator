#include <glad/glad.h>

#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "camera.hpp"

#include <cmath>



Camera::Camera(glm::vec3 pos, glm::vec3 world_up,
	       float pitch, float yaw, float roll,
	       float rotation_speed, float movement_speed) : 
			position(pos), world_up(world_up),
		       	pitch(pitch), yaw(yaw), roll(roll), rotation_speed(rotation_speed), movement_speed(movement_speed){
	
		this->npitch = this->nyaw = this-> nroll = 0.0f;
		this->forwards = glm::vec3(0,0,-1);
		this->up = world_up;
		this->leftRight = glm::cross(this->forwards,world_up);

		this->update();
}

void Camera::processAction(ObjectAction action){		 
		switch (action){
			case OBJ_ORBIT_LEFT:
			       	this->nyaw += this->rotation_speed;	
				break;
			case OBJ_ORBIT_RIGHT:
			       	this->nyaw -= this->rotation_speed;
				break;
		}
		this->calculateOrbitMovement();
}

void Camera::calculateOrbitMovement(){
	float dX = glm::cos(this->nyaw/2);
       	float dZ = glm::sin(this->nyaw/2);	
	if (this->yaw > M_PI) {
		dZ *= -1;
	}
	if ( M_PI_2 < this->yaw  && this->yaw > E_TAU-M_PI_2 ){
		dX *= -1;
	}

	this->velocity.x += dX;
	this->velocity.z += dZ;
}

void Camera::update(){
	glm::mat4 vectorMatrix(this->position.x, this->position.y, this->position.z, 0,
			       this->forwards.x, this->forwards.y, this->forwards.z, 0,
			       this->leftRight.x, this->leftRight.y, this->leftRight.z, 0,
			       this->up.x, this->up.y, this->up.z, 0);

	vectorMatrix = glm::rotate(vectorMatrix,this->npitch, this->leftRight);
	vectorMatrix = glm::rotate(vectorMatrix,this->nyaw,this->up);
	vectorMatrix = glm::rotate(vectorMatrix,this->nroll,this->forwards);

	this->yaw += this->nyaw;
	this->pitch += this->npitch;
	this->roll += this->nroll;

	this->overflowRotations();

	this->position += this->velocity;
	this->velocity = glm::vec3(0,0,0);
	this->nyaw = this->npitch = this->nroll = 0;
	
	this->position = glm::normalize(vectorMatrix[0].xyz());
	this->forwards = glm::normalize(vectorMatrix[1].xyz());
	this->leftRight = glm::normalize(vectorMatrix[2].xyz());
	this->up = glm::normalize(vectorMatrix[3].xyz());
}

glm::mat4 Camera::createViewMatrix(){
	return glm::lookAt(this->position, this->position+this->forwards, this->up);
}

