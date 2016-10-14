#pragma once
#include "PhysicsObject.h"
class Projectile : public PhysicsObject
{
public:
	Projectile(glm::vec3 a_position, float a_mass, float timeStep, glm::vec3 a_force);
	~Projectile();
	Projectile();
	void create();
private:
	// P1 = P + (V * T) -- Final pos equals starting pos + (Velocity * time in motion) t = deltaTime most likely
};

