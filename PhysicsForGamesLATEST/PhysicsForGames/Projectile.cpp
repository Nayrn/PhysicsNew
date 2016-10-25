#include "Projectile.h"

Projectile::Projectile(glm::vec3 a_position, float a_mass, float timeStep, glm::vec3 a_force)
{
	m_position = a_position;
	m_massPO = a_mass;
	m_deltaTime = timeStep;
	hasRB = true;
}

Projectile::~Projectile()
{
	delete this;
}

Projectile::Projectile()
{
}

void Projectile::create()
{
}
