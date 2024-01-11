#pragma once
#include <PxSimulationEventCallback.h>
#include <PxPhysicsAPI.h>
#include <PxSimulationEventCallback.h>
#include "Particle.h"
using namespace physx;

class MyCallBacks : public PxSimulationEventCallback
{
	const float minLinearSpeed = 7.0f;
	const float minAngularSpeed = 7.0f;
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
			if(particle2->getRigid()->getLinearVelocity().magnitude() > minLinearSpeed || particle2->getRigid()->getAngularVelocity().magnitude() > minAngularSpeed)
				particle1->setTirado();
			if(particle1->getRigid()->getLinearVelocity().magnitude() > minLinearSpeed || particle1->getRigid()->getAngularVelocity().magnitude() > minAngularSpeed)
			particle2->setTirado();
		}
		else if (particle1 == nullptr) {
			bola = (RigidParticle*)ac1->userData;
			if (bola != nullptr && bola->getRigid()->getLinearVelocity().magnitude() > minLinearSpeed)
				particle2->setTirado();
		}
		else if (particle2 == nullptr) {
			bola = (RigidParticle*)ac2->userData;
			if (bola != nullptr && bola->getRigid()->getLinearVelocity().magnitude() > minLinearSpeed)
				particle1->setTirado();
		}
	}
	//This is called with the current trigger pair events.
	virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) override { };
	//Provides early access to the new pose of moving rigid bodies.
	virtual void onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) override {};
};