#include "Projectile.h"

Projectile::Projectile(glm::vec3 a_position, float a_mass, float timeStep, glm::vec3 a_force)
{
	m_position = a_position;
	m_massPO = a_mass;
	hasRB = true;
	m_shapeID = SPHERE;
}

Projectile::~Projectile()
{
}

Projectile::Projectile()
{
}

void Projectile::create()
{
	makeGizmo();	
}
