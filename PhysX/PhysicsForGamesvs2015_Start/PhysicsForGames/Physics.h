#ifndef SOUND_PROGRAMMING_H_
#define SOUND_PROGRAMMING_H_

#include "Application.h"
#include "Camera.h"
#include "Render.h"
#include"ParticleEmitter.h"
#include "ParticleFluidEmitter.h"
#include <PxPhysicsAPI.h>
#include <PxScene.h>
#include <pvd/PxVisualDebugger.h>
#include <atomic>
using namespace physx;

const PxVec3 X_AXIS = PxVec3(1, 0, 0);
const PxVec3 Y_AXIS = PxVec3(0, 1, 0);
const PxVec3 Z_AXIS = PxVec3(0, 0, 1);


class MycollisionCallBack : public PxSimulationEventCallback
{
public: // fuck
	virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) {}
	virtual void onTrigger(PxTriggerPair* pairs, PxU32 nbPairs);
	virtual void	onConstraintBreak(PxConstraintInfo*, PxU32) {};
	virtual void	onWake(PxActor**, PxU32) {};
	virtual void	onSleep(PxActor**, PxU32) {};
};

class MyControllerHitReport : public PxUserControllerHitReport
{
public :

	//overload the onShapeHit function
	virtual void 	onShapeHit(const PxControllerShapeHit &hit);
	//other collision functions which we must overload	//these handle collision with other controllers and hitting obstacles
	virtual void 	onControllerHit(const PxControllersHit &hit) {};
	//Called when current controller hits another controller. More...
	virtual void 	onObstacleHit(const PxControllerObstacleHit &hit) {};
	//Called when current controller hits a user-defined obstacl
	MyControllerHitReport() :PxUserControllerHitReport() {};

	PxVec3 get_PlayerContactNormal() { return _playerContactNormal; };
	void clearPlayerContactNormal() { _playerContactNormal = PxVec3(0, 0, 0); };
	PxVec3 _playerContactNormal;
};




struct FilterGroup
{
	enum Enum
	{
		ePLAYER = (1 << 0),
		ePLATFORM = (1 << 1),
		eGROUND = (1 << 2)
	};
};


enum RagDollParts
{
	NO_PARENT = -1,
	LOWER_SPINE,
	LEFT_PELVIS,
	RIGHT_PELVIS,
	LEFT_UPPER_LEG,
	RIGHT_UPPER_LEG,
	LEFT_LOWER_LEG,
	RIGHT_LOWER_LEG,
	UPPER_SPINE,
	LEFT_CLAVICLE,
	RIGHT_CLAVICLE,
	NECK,
	HEAD,
	LEFT_UPPER_ARM,
	RIGHT_UPPER_ARM,
	LEFT_LOWER_ARM,
	RIGHT_LOWER_ARM,
};


struct RagdollNode
{
	PxQuat globalRotation;
	PxVec3 scaledGlobalPos;

	int parentNodeIdx;
	float halfLength;
	float radius;

	float parentLinkPos;
	char* name;
	float childLinkPos;
	PxArticulationLink* linkPtr;
	//longest constructor ever
	RagdollNode(PxQuat _globalRotation, int _parentNodeIdx, float _halfLength, float _radius, float _parentLinkPos, float _childLinkPos, char* _name) { globalRotation = _globalRotation, parentNodeIdx = _parentNodeIdx; halfLength = _halfLength; radius = _radius; parentLinkPos = _parentLinkPos; childLinkPos = _childLinkPos; name = _name; };
};

class Physics : public Application
{



	// go to Introduction to Physics first, then to collision thingies
public:
	virtual bool startup();
	virtual void shutdown();
    virtual bool update();
    virtual void draw();

	void renderGizmos(PxScene* physics_scene);
	void setUpPhysX();
	void updatePhysX(float _deltaTime);
	void setUpVisDebugger();
	void setUpTutorial();
	void setUpIntroductionToPhysX();

	float _characterYVelocity;
	float _characterRotation;
	float _playerGravity;

    Renderer* m_renderer;
    FlyCamera m_camera;
    float m_delta_time;
	std::vector<PxRigidActor*> g_PhysXActors;

	PxFoundation* g_PhysicsFoundation;
	PxPhysics* g_Physics;
	PxScene* g_PhysicsScene;
	PxDefaultErrorCallback gDefaultErrorCallback;
	PxDefaultAllocator gDefaultAllocatorCallback;
	PxSimulationFilterShader gDefaultFilterShader = PxDefaultSimulationFilterShader;
	PxMaterial* g_PhysicsMaterial;
	PxMaterial* g_boxMaterial;
	PxCooking* g_PhysicsCooker;
	PxScene* createDefaultScene();
	MyControllerHitReport* myHitReport;
	PxArticulation* makeRagdoll(PxPhysics* g_physics, RagdollNode** nodeArray, PxTransform worldPos, float scaleFactor, PxMaterial* ragdollMaterial);
	PxController* g_PlayerController;
	PxControllerManager* g_ControllerManager;
	//ParticleEmitter* m_particleEmitter;
	ParticleFluidEmitter* m_particleEmitter;
};





#endif //CAM_PROJ_H_
