

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>



#define E_TAU 6.28318530718f

enum ObjectAction : uint {
	OBJ_ORBIT_UP,
	OBJ_ORBIT_DOWN,
	OBJ_ORBIT_LEFT,
	OBJ_ORBIT_RIGHT,
	OBJ_PAN_UP,
	OBJ_PAN_DOWN,
	OBJ_PAN_LEFT,
	OBJ_PAN_RIGHT,
	OBJ_ROTATE_LEFT,
	OBJ_ROTATE_RIGHT,
	OBJ_ROLL_LEFT,
	OBJ_ROLL_RIGHT
};

enum ObjectTransformation : uint{
	OBJ_ROTATE,
	OBJ_MOVE
};


class Camera{
	private:
		glm::vec3 position{};
		glm::vec3 up{};
		glm::vec3 world_up{};
		glm::vec3 forwards{};
		glm::vec3 leftRight{};
		glm::vec3 velocity{};
		float pitch, yaw, roll;
		float npitch, nyaw, nroll;
		float rotation_speed, movement_speed;
		 //0,0,0 will point towards positive -z axis

		inline void overflowRotations(){
			this->pitch = std::fmod(this->pitch, E_TAU);
			this->yaw = std::fmod(this->yaw, E_TAU);
			this->roll = std::fmod(this->roll, E_TAU);
		};

		void calculateOrbitMovement();
	public:
		Camera(glm::vec3 pos, glm::vec3 world_up,
		       float pitch, float yaw, float roll,
		       float rotation_speed, float movement_speed);
		
		void processAction(ObjectAction action);
		void update();

		glm::vec3 getPosition(){ return this->position;}
		glm::mat4 createViewMatrix();
};


#endif
