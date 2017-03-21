#ifndef SOUND_PROGRAMMING_H_
#define SOUND_PROGRAMMING_H_

#include "Application.h"
#include "Camera.h"
#include "Render.h"

#include <PxPhysicsAPI.h>
#include <PxScene.h>
#include "DIYPhysicsScene.h"

using namespace physx;
class Physics : public Application
{
public:
	virtual bool startup();
	virtual void shutdown();
    virtual bool update();
    virtual void draw();

	void renderGizmos(PxScene* physics_scene);
	void DestroyProj();


    Renderer* m_renderer;
    FlyCamera m_camera;
    float m_delta_time;
	DIYPhysicsScene* m_pScene;
	Sphere* projectile;
	bool projAwake;
	float projLifeTime;
};



#endif //CAM_PROJ_H_
