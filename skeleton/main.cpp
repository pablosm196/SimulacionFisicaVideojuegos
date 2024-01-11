#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"
#include "Particle.h"
#include "Projectile.h"
#include "ParticleSystem.h"
#include "SolidSystem.h"
#include <iostream>

std::string display_text = "This is a test";


using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;


PxMaterial*				gMaterial	= NULL;

PxPvd*                  gPvd        = NULL;

PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene      = NULL;
ContactReportCallback gContactReportCallback;


bool torbellino, wind, gravity;

PxRigidStatic* suelo;

ParticleSystem* Psystem = nullptr;
SolidSystem* SolidSys = nullptr;

// Initialize physics engine
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport,PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(),true,gPvd);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);


	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);

	Psystem = new ParticleSystem(gScene, gPhysics);
	torbellino = false;
	wind = false;
	gravity = false;

	//Suelo (sólido rígido)
	suelo = gPhysics->createRigidStatic(PxTransform({ 0, 0, 0 }));
	PxShape* plano = CreateShape(PxBoxGeometry(200, 1, 100));
	suelo->attachShape(*plano);
	gScene->addActor(*suelo);
	RenderItem* sueloItem = new RenderItem(plano, suelo, { 1, 1, 0, 1 });


	/*PxRigidDynamic* pelota = gPhysics->createRigidDynamic(PxTransform({0, 50, 0}));
	pelota->setLinearVelocity({ 0, 2, 0 });
	PxShape* esfera = CreateShape(PxSphereGeometry(5));
	pelota->attachShape(*esfera);
	PxRigidBodyExt::updateMassAndInertia(*pelota, 1);
	gScene->addActor(*pelota);
	RenderItem* pelotaItem = new RenderItem(esfera, pelota, { 0, 1, 0, 1 });*/

	SolidSys = new SolidSystem(gScene, gPhysics);
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	gScene->simulate(t);
	gScene->fetchResults(true);

	Psystem->update(t);
	SolidSys->update(t);
	if (SolidSys->getWin())
		Psystem->setFireworkGenerator(true);
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	//delete particle;
	delete Psystem;
	delete SolidSys;

	PX_UNUSED(interactive);

	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	gScene->release();
	gDispatcher->release();
	// -----------------------------------------------------
	gPhysics->release();	
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();
	
	gFoundation->release();
}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	SolidSys->keyPress(key);

	switch (toupper(key))
	{
		//case 'B': break;
		//case ' ':	break;
	case 'T': {
		torbellino = !torbellino;
		//Psystem->setTorbellino(torbellino);
		break;
	}
	case 'V': {
		wind = !wind;
		//Psystem->setWind(wind);
		break;
	}
	case 'G': {
		gravity = !gravity;
		//Psystem->setGravity(gravity);
		break;
	}
	default:
		break;
	}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
}


int main(int, const char*const*)
{
#ifndef OFFLINE_EXECUTION 
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for(PxU32 i=0; i<frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}