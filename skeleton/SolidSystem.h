#pragma once
#include"ParticleGenerator.h";
#include "ParticleForceRegistry.h"
#include "ForceGenerator.h"
#include "Trigger.h"
#include<vector>
#include "MyCallBacks.h"
class SolidSystem
{
private:
	RigidParticle* ball;
	float force;
	bool canThrow, win;
	std::vector<std::vector<Bolo*>> bolos;
	std::vector<PxRigidStatic*> walls;
	std::vector<Particle*> railes;
	BallTrigger* ballTrigger, *ballTrigger2;
	CameraTrigger* cameraTrigger, *cameraTrigger2;
	void generateBall();
	void generateBolos(Vector3 pos);
	void generateWalls(Vector3 pos);
	PxScene* scene;
	PxPhysics* physics;
	ParticleForceRegistry* ForceRegistry;
	std::vector<ForceGenerator*> forces;
	MyCallBacks* callback = nullptr;
public:
	SolidSystem(PxScene* s, PxPhysics* p);
	~SolidSystem();
	void update(double t);
	void keyPress(unsigned char key);
	inline bool getWin() { return win; }
};

