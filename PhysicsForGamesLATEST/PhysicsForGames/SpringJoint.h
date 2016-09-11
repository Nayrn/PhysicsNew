#pragma once
#include "PhysicsObject.h"
#include "RigidBody.h"

class SpringJoint : public PhysicsObject
{
public:
	SpringJoint(RigidBody* connection1, RigidBody* connection2, float springCoefficient, float damping);
	SpringJoint();
	~SpringJoint();
	void update(glm::vec3 a_gravity, float timeStep) override;
	void debug() override;
	void makeGizmo() override;
	void resetPosition(glm::vec3 m_vNewPos) override { m_position = m_vNewPos; };
	void draw(glm::vec3 pos_a, glm::vec3 pos_b);
	glm::vec3 m_vNewPos;
	//void Draw(); just put in the line connecting gizmos
protected:

private:
	RigidBody* m_connections[2];
	float m_damping;
	float m_restLength;
	float m_springCoefficient;


};