#include "Physics.h"

#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"
#include "Gizmos.h"
#include<iostream>
#include "glm/ext.hpp"
#include "glm/gtc/quaternion.hpp"
#include "myAllocator.h"

#define Assert(val) if (val){}else{ *((char*)0) = 0;}
#define ArrayCount(val) (sizeof(val)/sizeof(val[0]))


RagdollNode* ragdollData[] =
{
	new RagdollNode(PxQuat(PxPi / 2.0f, Z_AXIS), NO_PARENT,1,3,1,1,"lower spine"),
	new RagdollNode(PxQuat(PxPi, Z_AXIS), LOWER_SPINE, 1,1,-1,1,"left pelvis"),
	new RagdollNode(PxQuat(0, Z_AXIS), LOWER_SPINE, 1,1,-1, 1,"right pelvis"),
	new RagdollNode(PxQuat(PxPi / 2.0f + 0.2f, Z_AXIS),LEFT_PELVIS,5,2,-1,1,"L upper leg"),
	new RagdollNode(PxQuat(PxPi / 2.0f - 0.2f, Z_AXIS),RIGHT_PELVIS,5,2,-1,1,"R upper leg"),
	new RagdollNode(PxQuat(PxPi / 2.0f + 0.2f, Z_AXIS),LEFT_UPPER_LEG,5,1.75,-1,1,"L lower leg"),
	new RagdollNode(PxQuat(PxPi / 2.0f - 0.2f, Z_AXIS),RIGHT_UPPER_LEG,5,1.75,-1,1,"R lowerleg"),
	new RagdollNode(PxQuat(PxPi / 2.0f, Z_AXIS), LOWER_SPINE, 1, 3, 1, -1, "upper spine"),
	new RagdollNode(PxQuat(PxPi, Z_AXIS), UPPER_SPINE, 1, 1.5, 1, 1, "left clavicle"),
	new RagdollNode(PxQuat(0, Z_AXIS), UPPER_SPINE, 1, 1.5, 1, 1, "right clavicle"),
	new RagdollNode(PxQuat(PxPi / 2.0f, Z_AXIS), UPPER_SPINE, 1, 1, 1, -1, "neck"),
	new RagdollNode(PxQuat(PxPi / 2.0f, Z_AXIS), NECK, 1, 3, 1, -1, "HEAD"),
	new RagdollNode(PxQuat(PxPi - .3, Z_AXIS), LEFT_CLAVICLE, 3, 1.5, -1, 1, "left upper arm"),
	new RagdollNode(PxQuat(0.3, Z_AXIS), RIGHT_CLAVICLE, 3, 1.5, -1, 1, "right upper arm"),
	new RagdollNode(PxQuat(PxPi - .3, Z_AXIS), LEFT_UPPER_ARM, 3, 1, -1, 1, "left lower arm"),
	new RagdollNode(PxQuat(0.3, Z_AXIS), RIGHT_UPPER_ARM, 3, 1, -1, 1, "right lower arm"),
	NULL
};



void setupFiltering(PxRigidActor* actor, PxU32 filterGroup, PxU32 filterMask)
{
	PxFilterData filterData;
	filterData.word0 = filterGroup; // word0 = own ID
	filterData.word1 = filterMask;  // word1 = ID mask to filter pairs that trigger a contact callback;
	const PxU32 numShapes = actor->getNbShapes();
	PxShape** shapes = (PxShape**)_aligned_malloc(sizeof(PxShape*)*numShapes, 16);
	actor->getShapes(shapes, numShapes);
	for (PxU32 i = 0; i < numShapes; i++)
	{
		PxShape* shape = shapes[i];
		shape->setSimulationFilterData(filterData);
	}
	_aligned_free(shapes);
}


bool Physics::startup()
{
	
    if (Application::startup() == false)
    {
        return false;
    }
	
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    Gizmos::create();

    m_camera = FlyCamera(1280.0f / 720.0f, 10.0f);
    m_camera.setLookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
    m_camera.sensitivity = 3;
	m_renderer = new Renderer();
	setUpPhysX();
	setUpVisDebugger();
	



	myHitReport = new MyControllerHitReport();
	g_ControllerManager = PxCreateControllerManager(*g_PhysicsScene);
	//describe our controller...
	PxCapsuleControllerDesc desc;
	desc.height = 1.6f;
	desc.radius = 0.4f;
	desc.position.set(0, 10, 0);
	desc.material = g_PhysicsMaterial;
	desc.reportCallback = myHitReport;  //connect it to our collision detection routine
	desc.density = 10;
	//create the player controller
	g_PlayerController = g_ControllerManager->createController(desc);

	g_PlayerController->setPosition(PxExtendedVec3(50, 1, 1));
	//set up some variables to control our player with
	_characterYVelocity = 0;  //initialize character velocity
	_characterRotation = 0; //and rotation
	_playerGravity = -0.5f; //set up the player gravity
	myHitReport->clearPlayerContactNormal(); //initialize the contact normal (what we are in contact with)
	//addToActorList(g_PlayerController->getActor());

	

	PxArticulation* ragdollArticulation;
	ragdollArticulation = makeRagdoll(g_Physics, ragdollData, PxTransform(PxVec3(0, 50, 0)), .3f, g_PhysicsMaterial);
	g_PhysicsScene->addArticulation(*ragdollArticulation);
	
	
	PxSimulationEventCallback* mycollisionCallBack = new MycollisionCallBack();
	g_PhysicsScene->setSimulationEventCallback(mycollisionCallBack);



	// trigger stuff -- don't forget to uncomment
	//setupFiltering(g_PlayerController->getActor(), FilterGroup::ePLAYER, FilterGroup::eGROUND);  //set up the collision filtering for our player
	
	  //set up the collision filtering for our player with ground and platform.

	PxTransform pose = PxTransform(PxVec3(0.0f, 0, 0.0f), PxQuat(PxHalfPi,
		PxVec3(0.0f, 0.0f, 1.0f)));

	PxRigidStatic* plane = PxCreateStatic(*g_Physics, pose, PxPlaneGeometry(),
		*g_PhysicsMaterial);
	const PxU32 numShapes = plane->getNbShapes();
	g_PhysicsScene->addActor(*plane);

	setupFiltering(plane, FilterGroup::eGROUND, FilterGroup::ePLAYER);
	setupFiltering(g_PlayerController->getActor(), FilterGroup::ePLAYER, FilterGroup::eGROUND | FilterGroup::ePLATFORM);

	PxBoxGeometry side1(4.5, 1, .5);
	PxBoxGeometry side2(.5, 1, 4.5);
	pose = PxTransform(PxVec3(0.0f, 0.5, 4.0f));
	PxRigidStatic* box = PxCreateStatic(*g_Physics, pose, side1,
		*g_PhysicsMaterial);

	g_PhysicsScene->addActor(*box);
	g_PhysXActors.push_back(box);

	pose = PxTransform(PxVec3(0.0f, 0.5, -4.0f));
	box = PxCreateStatic(*g_Physics, pose, side1, *g_PhysicsMaterial);
	g_PhysicsScene->addActor(*box);
	g_PhysXActors.push_back(box);

	pose = PxTransform(PxVec3(4.0f, 0.5, 0));
	box = PxCreateStatic(*g_Physics, pose, side2, *g_PhysicsMaterial);
	g_PhysicsScene->addActor(*box);
	g_PhysXActors.push_back(box);

	pose = PxTransform(PxVec3(-4.0f, 0.5, 0));
	box = PxCreateStatic(*g_Physics, pose, side2, *g_PhysicsMaterial);
	g_PhysicsScene->addActor(*box);
	g_PhysXActors.push_back(box);

	//PxParticleSystem* pf;
	//
	//// create particle system in PhysX SDK
	//// set immutable properties.
	//PxU32 maxParticles = 4000;
	//bool perParticleRestOffset = false;
	//pf = g_Physics->createParticleSystem(maxParticles, perParticleRestOffset);
	//
	//
	//if (pf)
	//{
	//	g_PhysicsScene->addActor(*pf);
	//	m_particleEmitter = new ParticleEmitter(maxParticles,
	//		PxVec3(0, 10, 0), pf, .01);
	//	m_particleEmitter->setStartVelocityRange(-2.0f, 0, -2.0f,
	//		2.0f, 0.0f, 2.0f);
	//}
	//

	/*PARTICLE EMITTER*/
	//create our particle system
	PxParticleFluid* pf;
	// create particle system in PhysX SDK
	// set immutable properties.
	PxU32 maxParticles = 4000;
	bool perParticleRestOffset = false;
	pf = g_Physics->createParticleFluid(maxParticles, perParticleRestOffset);

	pf->setRestParticleDistance(.3f);
	pf->setDynamicFriction(0.1);
	pf->setStaticFriction(0.1);
	pf->setDamping(0.1);
	pf->setParticleMass(.1);
	pf->setRestitution(0);
	pf->setParticleReadDataFlag(PxParticleReadDataFlag::eDENSITY_BUFFER, true);
	pf->setParticleBaseFlag(PxParticleBaseFlag::eCOLLISION_TWOWAY, true);
	pf->setStiffness(100);

	if (pf)
	{
		g_PhysicsScene->addActor(*pf);
		m_particleEmitter = new ParticleFluidEmitter(maxParticles,
			PxVec3(0, 10, 0), pf, .1);
		m_particleEmitter->setStartVelocityRange(-0.001f, -250.0f, -0.001f,
			0.001f, -250.0f, 0.001f);
	}


    return true;
}

void Physics::shutdown()
{
	g_PhysicsScene->release();
	g_Physics->release();
	g_PhysicsFoundation->release();
	delete m_renderer;
    Gizmos::destroy();
    Application::shutdown();
}

bool Physics::update()
{
    if (Application::update() == false)
    {
        return false;
    }

    Gizmos::clear();

    float dt = (float)glfwGetTime();
    m_delta_time = dt;
    glfwSetTime(0.0);

    vec4 white(1);
    vec4 black(0, 0, 0, 1);
	
    for (int i = 0; i <= 20; ++i)
    {
        Gizmos::addLine(vec3(-10 + i, -0.01, -10), vec3(-10 + i, -0.01, 10),
            i == 10 ? white : black);
        Gizmos::addLine(vec3(-10, -0.01, -10 + i), vec3(10, -0.01, -10 + i),
            i == 10 ? white : black);
    }

    m_camera.update(1.0f / 60.0f);
	updatePhysX(m_delta_time);
	for (auto actor : g_PhysXActors)
	{
		{
			PxU32 nShapes = actor->getNbShapes();
			PxShape** shapes = new PxShape*[nShapes];
			actor->getShapes(shapes, nShapes);
			// Render all the shapes in the physx actor (for early tutorials there is just one)
			while (nShapes--)
			{
				//AddWidget(shapes[nShapes], actor, glm::vec4(0, 1, 0, 1));
			}
			delete[] shapes;
		}
	}

	bool onGround; //set to true if we are on the ground
	float movementSpeed = 10.0f; //forward and back movement speed
	float rotationSpeed = 1.0f; //turn speed
	myHitReport = new MyControllerHitReport();							//check if we have a contact normal.  if y is greater than .3 we assume this is solid ground.  This is a rather primitive way to do this.  Can you do better?
	if (myHitReport->get_PlayerContactNormal().y > 0.3f)
	{
		_characterYVelocity = -0.1f;
		onGround = true;
	}
	else
	{
		_characterYVelocity += _playerGravity * m_delta_time;
		onGround = false;
	}
	myHitReport->clearPlayerContactNormal();
	const PxVec3 up(0, 1, 0);
	//scan the keys and set up our intended velocity based on a global transform	
	PxVec3 velocity(0, _characterYVelocity, 0);
	if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS)
		velocity.x -= movementSpeed*m_delta_time;
	
	if (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS)
		velocity.x += movementSpeed*m_delta_time;
	
	if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS)
	
		velocity.z += movementSpeed*m_delta_time;
	
	if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS)

		velocity.z -= movementSpeed*m_delta_time;

	if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
		velocity.y += movementSpeed * m_delta_time * 5;



	float minDistance = 0.001f;
	PxControllerFilters filter;
	//make controls relative to player facing
	PxQuat rotation(_characterRotation, PxVec3(0, 1, 0));
	PxVec3 _velocity(0, _characterYVelocity, 0);
	//move the controller
	g_PlayerController->move(rotation.rotate(velocity), minDistance, m_delta_time, filter);

	if (m_particleEmitter)
	{
		m_particleEmitter->update(m_delta_time);
		//render all our particles
		m_particleEmitter->renderParticles();
	}

	

    return true;
}

void Physics::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	//Gizmos::clear();
    Gizmos::draw(m_camera.proj, m_camera.view);

    m_renderer->RenderAndClear(m_camera.view_proj);

    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void AddWidget(PxShape* shape, PxRigidActor* actor, vec4 geo_color)
{
    PxTransform full_transform = PxShapeExt::getGlobalPose(*shape, *actor);
    vec3 actor_position(full_transform.p.x, full_transform.p.y, full_transform.p.z);
    glm::quat actor_rotation(full_transform.q.w,
        full_transform.q.x,
        full_transform.q.y,
        full_transform.q.z);
    glm::mat4 rot(actor_rotation);

    mat4 rotate_matrix = glm::rotate(10.f, glm::vec3(7, 7, 7));

    PxGeometryType::Enum geo_type = shape->getGeometryType();

    switch (geo_type)
    {
    case (PxGeometryType::eBOX) :
    {
        PxBoxGeometry geo;
        shape->getBoxGeometry(geo);
        vec3 extents(geo.halfExtents.x, geo.halfExtents.y, geo.halfExtents.z);
        Gizmos::addAABBFilled(actor_position, extents, geo_color, &rot);
    } break;
    case (PxGeometryType::eCAPSULE) :
    {
        PxCapsuleGeometry geo;
        shape->getCapsuleGeometry(geo);
        Gizmos::addCapsule(actor_position, geo.halfHeight * 2, geo.radius, 16, 16, geo_color, &rot);

    } break;
    case (PxGeometryType::eSPHERE) :
    {
        PxSphereGeometry geo;
        shape->getSphereGeometry(geo);
        Gizmos::addSphereFilled(actor_position, geo.radius, 16, 16, geo_color, &rot);
    } break;
    case (PxGeometryType::ePLANE) :
    {

    } break;
    }
}

void Physics::renderGizmos(PxScene* physics_scene)
{
    PxActorTypeFlags desiredTypes = PxActorTypeFlag::eRIGID_STATIC | PxActorTypeFlag::eRIGID_DYNAMIC;
    PxU32 actor_count = physics_scene->getNbActors(desiredTypes);
    PxActor** actor_list = new PxActor*[actor_count];
	physics_scene->getActors(desiredTypes, actor_list, actor_count);
    //-- START
	PxTransform pose = PxTransform(PxVec3(0.0f, 0, 0.0f), PxQuat(PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f)));
	PxBoxGeometry side1(4.5, 1, .5);
	PxBoxGeometry side2(.5, 1, 4.5);

	pose = PxTransform(PxVec3(0.0f, 0.5, 4.0f));
	PxRigidStatic* box = PxCreateStatic(*g_Physics, pose, side1, *g_PhysicsMaterial);
//
	g_PhysicsScene->addActor(*box);
	g_PhysXActors.push_back(box);

    vec4 geo_color(1, 0, 0, 1);
    for (int actor_index = 0;
        actor_index < (int)actor_count;
        ++actor_index)
    {
        PxActor* curr_actor = actor_list[actor_index];
        if (curr_actor->isRigidActor())
        {
            PxRigidActor* rigid_actor = (PxRigidActor*)curr_actor;
            PxU32 shape_count = rigid_actor->getNbShapes();
            PxShape** shapes = new PxShape*[shape_count];
            rigid_actor->getShapes(shapes, shape_count);

            for (int shape_index = 0;
                shape_index < (int)shape_count;
                ++shape_index)
            {
                PxShape* curr_shape = shapes[shape_index];
                AddWidget(curr_shape, rigid_actor, geo_color);
            }

            delete[]shapes;
        }
    }

    delete[] actor_list;

    int articulation_count = physics_scene->getNbArticulations();

    for (int a = 0; a < articulation_count; ++a)
    {
        PxArticulation* articulation;
		physics_scene->getArticulations(&articulation, 1, a);

        int link_count = articulation->getNbLinks();

        PxArticulationLink** links = new PxArticulationLink*[link_count];
        articulation->getLinks(links, link_count);

        for (int l = 0; l < link_count; ++l)
        {
            PxArticulationLink* link = links[l];
            int shape_count = link->getNbShapes();

            for (int s = 0; s < shape_count; ++s)
            {
                PxShape* shape;
                link->getShapes(&shape, 1, s);
                AddWidget(shape, link, geo_color);
            }
        }
        delete[] links;
    }

//	for (auto articulation : g_PhysXActorsRagDolls)
//	{
//		PxU32 nLinks = articulation->getNbLinks();
//		PxArticulationLink** links = new PxArticulationLink*[nLinks];
//		articulation->getLinks(links, nLinks);
//		if (renderGizmos)
//		{
//			while (nLinks--)
//			{
//				PxArticulationLink* link = links[nLinks];
//				PxU32 nShapes = link->getNbShapes();
//				PxShape** shapes = new PxShape*[nShapes];
//				link->getShapes(shapes, nShapes);
//				while (nShapes--)
//				{
//					AddWidget(shapes[nShapes], link);
//				}
//			}
//			delete[] links;
//		}
//	}
//

}

void Physics::setUpPhysX()
{
	

	PxAllocatorCallback *myCallback = new myAllocator(); // create this class
	g_PhysicsFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, *myCallback, gDefaultErrorCallback);
	g_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *g_PhysicsFoundation, PxTolerancesScale());
	PxInitExtensions(*g_Physics);
	g_PhysicsMaterial = g_Physics->createMaterial(0.5f, 0.5f, .5f);
	PxSceneDesc sceneDesc(g_Physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0, -10.0f, 0);
	sceneDesc.filterShader = &physx::PxDefaultSimulationFilterShader;
	sceneDesc.cpuDispatcher = PxDefaultCpuDispatcherCreate(1);
	g_PhysicsScene = g_Physics->createScene(sceneDesc);

}

void Physics::updatePhysX(float _deltaTime)
{
	if (_deltaTime <= 0)
		return;

	g_PhysicsScene->simulate(_deltaTime);

	while (!g_PhysicsScene->fetchResults())
	{
		// do nothing just yet, need to fetch results tho
	}
	
}

void Physics::setUpVisDebugger()
{
	if (g_Physics->getPvdConnectionManager() == NULL)
		return;

	const char* pvd_host_ip = "127.0.0.1";
	int port = 5425;
	unsigned int timeOut = 100; // timeout in MS to wait for response

	PxVisualDebuggerConnectionFlags connectionFlags = PxVisualDebuggerExt::getAllConnectionFlags();

	auto theConnection = PxVisualDebuggerExt::createConnection(g_Physics->getPvdConnectionManager(), pvd_host_ip, port, timeOut, connectionFlags);
	setUpTutorial();
}

void setShapeAsTrigger(PxRigidActor* actorIn)
{
	PxRigidStatic* staticActor = actorIn->is<PxRigidStatic>();
	assert(staticActor); // throwing an error here

	const PxU32 numShapes = staticActor->getNbShapes();
	PxShape** shapes = (PxShape**)_aligned_malloc(sizeof(PxShape*)*numShapes, 16);
	staticActor->getShapes(shapes, numShapes);
	for (PxU32 i = 0; i < numShapes; i++)
	{
		shapes[i]->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
		shapes[i]->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	}
}

void Physics::setUpTutorial()
{
	// adding a plane
	PxTransform pose = PxTransform(PxVec3(0.0f, 0, 0.0f), PxQuat(PxHalfPi*1.0f, PxVec3(0.0f, 0.0f, 1.0f)));
	PxRigidStatic* plane = PxCreateStatic(*g_Physics, pose, PxPlaneGeometry(), *g_PhysicsMaterial);

	g_PhysicsScene->addActor(*plane);
	// is actually there, just v large

	// box

	float density = 10;
	PxBoxGeometry box(2, 2, 2);
	PxTransform transform(PxVec3(30, 1, 0));
	
	
	PxRigidStatic* staticActor = PxCreateStatic(*g_Physics, transform, box, *g_PhysicsMaterial);
	g_PhysicsScene->addActor(*staticActor);
	setShapeAsTrigger(staticActor);

	setupFiltering(staticActor, FilterGroup::ePLATFORM, FilterGroup::ePLAYER);

}

PxScene* Physics::createDefaultScene()
{
	PxSceneDesc scene_desc(g_Physics->getTolerancesScale());
	scene_desc.gravity = PxVec3(0, -9.807f, 0);

//	scene_desc.filterShader = myFliterShader;
//	scene_desc.filterShader = &PxDefaultSimulationFilterShader;

	scene_desc.cpuDispatcher = PxDefaultCpuDispatcherCreate(8);
	PxScene* result = g_Physics->createScene(scene_desc);

	return result;
}

PxArticulation * Physics::makeRagdoll(PxPhysics * g_physics, RagdollNode ** nodeArray, PxTransform worldPos, float scaleFactor, PxMaterial * ragdollMaterial)
{
	PxArticulation *articulation = g_physics->createArticulation();
	RagdollNode** currentNode = nodeArray;

	while (*currentNode != NULL)
	{

		RagdollNode* currentNodePtr = *currentNode;
		RagdollNode* parentNode = nullptr;

		// get scaled values
		float radius = currentNodePtr->radius * scaleFactor;
		float halfLength = currentNodePtr->halfLength * scaleFactor;
		float childHalfLength = radius + halfLength;
		float parentHalfLength = 0; // only 0 for now

		PxArticulationLink* parentLinkPtr = NULL;
		currentNodePtr->scaledGlobalPos = worldPos.p;

		if (currentNodePtr->parentNodeIdx != -1)
		{
			// if there is a parent, work out local pos
			parentNode = *(nodeArray + currentNodePtr->parentNodeIdx);
			parentLinkPtr = parentNode->linkPtr;
			parentHalfLength = (parentNode->radius + parentNode->halfLength) * scaleFactor;

			//working out pos of node
			PxVec3 currentRelative = currentNodePtr->childLinkPos * currentNodePtr->globalRotation.rotate(PxVec3(childHalfLength, 0, 0));

			PxVec3 parentRelative = -currentNodePtr->parentLinkPos * parentNode->globalRotation.rotate(PxVec3(parentHalfLength, 0, 0));

			currentNodePtr->scaledGlobalPos = parentNode->scaledGlobalPos - (parentRelative + currentRelative);


		}

		//building transform for link
		PxTransform linkTransform = PxTransform(currentNodePtr->scaledGlobalPos, currentNodePtr->globalRotation);

		//create link
		PxArticulationLink* link = articulation->createLink(parentLinkPtr, linkTransform);

		//add pointer to this link in to ragdoll link
		currentNodePtr->linkPtr = link;
		float jointSpace = .01f;
		float capsuleHalfLength = (halfLength > jointSpace ? halfLength - jointSpace : 0) + .01f;

		PxCapsuleGeometry capsule(radius, capsuleHalfLength);
		link->createShape(capsule, *ragdollMaterial); // adds capsule collider
		PxRigidBodyExt::updateMassAndInertia(*link, 50.0f); // adds mass

		if (currentNodePtr->parentNodeIdx != -1)
		{
			PxArticulationJoint * joint = link->getInboundJoint();
			PxQuat frameRotation = parentNode->globalRotation.getConjugate() * currentNodePtr->globalRotation;

			//setting constraint frame

			PxTransform parentConstraintFrame = PxTransform(PxVec3(currentNodePtr->parentLinkPos * parentHalfLength, 0, 0), frameRotation);

			// set child constraint frame

			PxTransform thisConstraintFrame = PxTransform(PxVec3(currentNodePtr->childLinkPos * childHalfLength, 0, 0));

			// set poses for the joint so it's in the right place
			joint->setParentPose(parentConstraintFrame);
			joint->setChildPose(thisConstraintFrame);

			// no floppy
			joint->setStiffness(20);
			joint->setDamping(20);
			joint->setSwingLimit(0.4f, 0.4f);
			joint->setSwingLimitEnabled(true);
			joint->setTwistLimit(-0.1f, 0.1f);
			joint->setTwistLimitEnabled(true);

		}

		currentNode++;
	}

	int articulation_count = g_PhysicsScene->getNbArticulations();
	vec4 geo_color(1, 0, 0, 1);
	for (int a = 0; a < articulation_count; ++a)
	{
		PxArticulation* articulation;
		g_PhysicsScene->getArticulations(&articulation, 1, a);

		int link_count = articulation->getNbLinks();

		PxArticulationLink** links = new PxArticulationLink*[link_count];
		articulation->getLinks(links, link_count);

		for (int l = 0; l < link_count; ++l)
		{
			PxArticulationLink* link = links[l];
			int shape_count = link->getNbShapes();

			for (int s = 0; s < shape_count; ++s)
			{
				PxShape* shape;
				link->getShapes(&shape, 1, s);
				AddWidget(shape, link, geo_color);
			}
		}
		delete[] links;
	}


	return articulation;

}

void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
	for (PxU32 i = 0; i < nbPairs; i++)
	{
		const PxContactPair& cp = pairs[i];
		//only interested in touches found and lost
		if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			std::cout << "Collision Detected between: ";
			std::cout << pairHeader.actors[0]->getName();
			std::cout << pairHeader.actors[1]->getName() << std::endl;

		}
	}
}


void MycollisionCallBack::onTrigger(PxTriggerPair* pairs, PxU32 nbPairs)
{
	for (PxU32 i = 0; i < nbPairs; i++)
	{
		PxTriggerPair* pair = pairs + i;
		PxActor* triggerActor = pair->triggerActor;
		PxActor* otherActor = pair->otherActor;
		//std::cout << otherActor->getName();
		std::cout << " Entered Trigger ";
		//std::cout << triggerActor->getName() << std::endl;
	}
};


PxFilterFlags myFliterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0,
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	// let triggers through
	if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
	{
		pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
		return PxFilterFlag::eDEFAULT;
	}
	// generate contacts for all that were not filtered above
	pairFlags = PxPairFlag::eCONTACT_DEFAULT;
	// trigger the contact callback for pairs (A,B) where
	// the filtermask of A contains the ID of B and vice versa.
	if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
		pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_LOST;

	return PxFilterFlag::eDEFAULT;
}


//helper function to set up filtering





void MyControllerHitReport::onShapeHit(const PxControllerShapeHit &hit)
{
	//gets a reference to a structure which tells us what has been hit and where
	//get the acter from the shape we hit
	PxRigidActor* actor = hit.shape->getActor();
	//get the normal of the thing we hit and store it so the player controller can respond correctly
	_playerContactNormal = hit.worldNormal;
	//try to cast to a dynamic actor
	PxRigidDynamic* myActor = actor->is<PxRigidDynamic>();
	if (myActor)
	{
		//this is where we can apply forces to things we hit
		std::cout << "hit" << std::endl;
	}
}



void Physics::setUpIntroductionToPhysX()
{
	g_PhysicsScene = createDefaultScene();
	
	//add a plane
	PxTransform pose = PxTransform(PxVec3(0.0f, 0, 0.0f), PxQuat(PxHalfPi*1.0f,
		PxVec3(0.0f, 0.0f, 1.0f)));
	PxRigidStatic* plane = PxCreateStatic(*g_Physics, pose, PxPlaneGeometry(),
		*g_PhysicsMaterial);
	//add it to the physX scene
	g_PhysicsScene->addActor(*plane);

	g_PhysicsScene->addActor(*PxCreateDynamic(*g_Physics, PxTransform(1, 0, 0), PxSphereGeometry(2), *g_PhysicsMaterial, 10));
}


