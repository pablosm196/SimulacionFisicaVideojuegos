#pragma once
#include <PxSimulationEventCallback.h>
#include <PxPhysicsAPI.h>
#include <PxSimulationEventCallback.h>
#include "Particle.h"
using namespace physx;

class MyCallBacks : public PxSimulationEventCallback
{
public:
	//This is called when a breakable constraint breaks.
	virtual void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) override {};
	//This is called with the actors which have just been woken up.
	virtual void onWake(PxActor** actors, PxU32 count) override {};
	//This is called with the actors which have just been put to sleep.
	virtual void onSleep(PxActor** actors, PxU32 count) override {};
	//This is called when certain contact events occur.
	virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override {
		PxRigidActor* ac1 = pairHeader.actors[0];
		PxRigidActor* ac2 = pairHeader.actors[1];

		Bolo* particle1 = (Bolo*)ac1->userData;
		Bolo* particle2 = (Bolo*)ac2->userData;
		RigidParticle* bola = nullptr;

		if (particle1 != nullptr && particle2 != nullptr) {
			particle1->setTirado();
			particle2->setTirado();
		}
		else if (particle1 == nullptr) {
			bola = (RigidParticle*)ac1->userData;
			if (bola != nullptr)
				particle2->setTirado();
		}
		else if (particle2 == nullptr) {
			bola = (RigidParticle*)ac2->userData;
			if (bola != nullptr)
				particle1->setTirado();
		}
	}
	//This is called with the current trigger pair events.
	virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) override { };
	//Provides early access to the new pose of moving rigid bodies.
	virtual void onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) override {};
};