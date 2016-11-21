//#include "RagDoll.h"
//#include <iostream>
//
//PxArticulation * RagDoll::makeRagdoll(PxPhysics * g_physics, RagdollNode ** nodeArray, PxTransform worldPos, float scaleFactor, PxMaterial * ragdollMaterial)
//{
//	PxArticulation *articulation = g_physics->createArticulation();
//	RagdollNode** currentNode = nodeArray;
//
//	while (*currentNode != NULL)
//	{
//		currentNode++;
//
//		RagdollNode* currentNodePtr = *currentNode;
//		RagdollNode* parentNode = nullptr;
//
//		// get scaled values
//		float radius = currentNodePtr->radius * scaleFactor;
//		float halfLength = currentNodePtr->halfLength * scaleFactor;
//		float childHalfLength = radius + halfLength;
//		float parentHalfLength = 0; // only 0 for now
//
//		PxArticulationLink* parentLinkPtr = NULL;
//		currentNodePtr->scaledGlobalPos = worldPos.p;
//
//		if (currentNodePtr->parentNodeIdx != -1)
//		{
//			// if there is a parent, work out local pos
//			parentNode = *(nodeArray + currentNodePtr->parentNodeIdx);
//			parentLinkPtr = parentNode->linkPtr;
//			parentHalfLength = (parentNode->radius + parentNode->halfLength) * scaleFactor;
//
//			//working out pos of node
//			PxVec3 currentRelative = currentNodePtr->childLinkPos * currentNodePtr->globalRotation.rotate(PxVec3(childHalfLength, 0, 0));
//
//			PxVec3 parentRelative = -currentNodePtr->parentLinkPos * parentNode->globalRotation.rotate(PxVec3(parentHalfLength, 0, 0));
//
//			currentNodePtr->scaledGlobalPos = parentNode->scaledGlobalPos - (parentRelative + currentRelative);
//
//				
//		}
//
//		//building transform for link
//		PxTransform linkTransform = PxTransform(currentNodePtr->scaledGlobalPos, currentNodePtr->globalRotation);
//
//		//create link
//		PxArticulationLink* link = articulation->createLink(parentLinkPtr, linkTransform);
//
//		//add pointer to this link in to ragdoll link
//		currentNodePtr->linkPtr = link;
//		float jointSpace = .01f;
//		float capsuleHalfLength = (halfLength > jointSpace ? halfLength - jointSpace : 0) + .01f;
//
//		PxCapsuleGeometry capsule(radius, capsuleHalfLength); 
//		link->createShape(capsule, *ragdollMaterial); // adds capsule collider
//		PxRigidBodyExt::updateMassAndInertia(*link, 50.0f); // adds mass
//
//		if (currentNodePtr->parentNodeIdx != -1)
//		{
//			PxArticulationJoint * joint = link->getInboundJoint();
//			PxQuat frameRotation = parentNode->globalRotation.getConjugate() * currentNodePtr->globalRotation;
//
//			//setting constraint frame
//
//			PxTransform parentConstraintFrame = PxTransform(PxVec3(currentNodePtr->parentLinkPos * parentHalfLength, 0, 0), frameRotation);
//
//			// set child constraint frame
//
//			PxTransform thisConstraintFrame = PxTransform(PxVec3(currentNodePtr->childLinkPos * childHalfLength, 0, 0));
//
//			// set poses for the joint so it's in the right place
//			joint->setParentPose(parentConstraintFrame);
//			joint->setChildPose(thisConstraintFrame);
//
//			// no floppy
//			joint->setStiffness(20);
//			joint->setDamping(20);
//			joint->setSwingLimit(0.4f, 0.4f);
//			joint->setSwingLimitEnabled(true);
//			joint->setTwistLimit(-0.1f, 0.1f);
//			joint->setTwistLimitEnabled(true);
//
//		}
//	}
//
//
//	return articulation;
//}
