#pragma once
#include <PxPhysicsAPI.h>
#include <PxScene.h>
#include <pvd/PxVisualDebugger.h>

using namespace physx;

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
	// page 5 of tutorial
};


class RagDoll
{
	

public:


private:

};