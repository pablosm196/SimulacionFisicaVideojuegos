#pragma once
#include"ParticleGenerator.h";
#include "ParticleForceRegistry.h"
#include "Trigger.h"
#include<vector>
class SolidSystem
{
private:
	RigidParticle* ball;
	float force;
	bool canThrow;
	std::vector<std::vector<RigidParticle*>> bolos;
	BallTrigger* ballTrigger;
	CameraTrigger* cameraTrigger;
	void generateBall();
	void generateBolos(Vector3 pos);
	PxScene* scene;
	PxPhysics* physics;
public:
	SolidSystem(PxScene* s, PxPhysics* p);
	~SolidSystem();
	void update(double t);
	void keyPress(unsigned char key);
};

