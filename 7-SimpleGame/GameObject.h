#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

#include "glm\glm.hpp"
#include "functionalities.h"


class GameObject
{
	GLchar type;  // 'e' for enemy, 'b' for bullet, 'p' for power
	glm::vec3 location;
	glm::vec3 look_at; // The 3D point in space that the must look
	GLfloat colliderRadius;
	GLfloat movementSpeed;
public:
	// type, collisionRadius, playerLocation, looking_direction
	GameObject(GLchar l_type, GLfloat l_colliderRadius, glm::vec3 l_startLocation, glm::vec3 l_looking_dir_vec, GLfloat l_movementSpeed)
	{
		this->type = l_type;
		this->colliderRadius = l_colliderRadius;
		this->location = l_startLocation;
		this->look_at = l_looking_dir_vec;

		movementSpeed = l_movementSpeed;
	}

	glm::vec3 getLocation() { return this->location; }

	bool checkIsBullet() { return 'b' == this->type; }

	bool checkIsEnemy() { return 'e' == this->type; }

	bool checkIsEnemyBullet() { return 'f' == this->type; }

	bool checkIsPowerPack() { return 'p' == this->type; }

	bool isCollidingWithPlayer() {

		return (abs(this->location.x - cam_pos.x) <= this->colliderRadius) &&
			(abs(this->location.y - cam_pos.y) <= this->colliderRadius);
	}

	bool isCollidingWith(glm::vec3 otherLocation) {
		return (abs(this->location.x - otherLocation.x) <= this->colliderRadius) &&
			(abs(this->location.y - otherLocation.y) <= this->colliderRadius);
	}

	void enemyFires() {
		std::this_thread::sleep_for(std::chrono::seconds(5));
		glm::vec3 bulletStartLocation = cam_pos * glm::vec3(1, 1, 0.0);
		GLfloat collisionRadius = 5;
		GLfloat bullSpeed = 0.02;
		generateProjectile('f', looking_dir_vector);
	}

	void incrementPosition() {
		this->location += (glm::vec3(this->movementSpeed, this->movementSpeed, 0) * this->look_at);
	}
};


#endif
