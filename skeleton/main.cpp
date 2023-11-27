#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"
#include "Particle.h"
#include "Projectile.h"
#include "ParticleSystem.h"
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
//Particle* particle;
std::vector<Projectile*> v;
ParticleSystem* Psystem;
bool torbellino, wind, gravity;

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
	//particle = new Particle(Vector3(10, 0, 0), Vector3(0, 0, 0), Vector3(0, -9.8f, 0), 0.998f);
	Psystem = new ParticleSystem();
	torbellino = false;
	wind = false;
	gravity = false;
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	gScene->simulate(t);
	gScene->fetchResults(true);
	//particle->integrate(t);
	//p->integrate(t);
	for (int i = 0; i < v.size(); ++i)
		v[i]->integrate(t);
	Psystem->update(t);
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	//delete particle;
	for (int i = 0; i < v.size(); ++i)
		delete v[i];
	delete Psystem;

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

	switch (toupper(key))
	{
		//case 'B': break;
		//case ' ':	break;
	case 'E': {
		/*Projectile* p = new Projectile(GetCamera()->getDir() * 330, GetCamera()->getDir() * 10, GetCamera()->getTransform().p, Vector3(0, -9.8f, 0), 0.998f, 0.2f);
		v.push_back(p);*/
		Psystem->generateExplosion();
		break;
	}
	case 'T': {
		torbellino = !torbellino;
		Psystem->setTorbellino(torbellino);
		break;
	}
	case 'V': {
		wind = !wind;
		Psystem->setWind(wind);
		break;
	}
	case 'G': {
		gravity = !gravity;
		Psystem->setGravity(gravity);
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