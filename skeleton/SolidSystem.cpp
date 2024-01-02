#include "SolidSystem.h"
#include<iostream>

void SolidSystem::generateBall()
{
	PxTransform transform = PxTransform(GetCamera()->getTransform().p);
	ball->getRigid()->setGlobalPose(transform);
	ball->addForce(force * GetCamera()->getDir().getNormalized());
	canThrow = false;
}

void SolidSystem::generateBolos(Vector3 pos)
{
	std::vector<RigidParticle*> p;

	p.push_back(new RigidParticle(Vector3(0, 0, 0), pos + Vector3(0, 0, -5), 1, physics, scene, 100.0f, Vector4(1, 1, 1, 1), BOX));
	p.push_back(new RigidParticle(Vector3(0, 0, 0), pos + Vector3(2.5, 0, 0), 1, physics, scene, 100.0f, Vector4(1, 1, 1, 1), BOX));
	p.push_back(new RigidParticle(Vector3(0, 0, 0), pos + Vector3(-2.5, 0, 0), 1, physics, scene, 100.0f, Vector4(1, 1, 1, 1), BOX));
	p.push_back(new RigidParticle(Vector3(0, 0, 0), pos + Vector3(0, 0, 5), 1, physics, scene, 100.0f, Vector4(1, 1, 1, 1), BOX));
	p.push_back(new RigidParticle(Vector3(0, 0, 0), pos + Vector3(5, 0, 5), 1, physics, scene, 100.0f, Vector4(1, 1, 1, 1), BOX));
	p.push_back(new RigidParticle(Vector3(0, 0, 0), pos + Vector3(-5, 0, 5), 1, physics, scene, 100.0f, Vector4(1, 1, 1, 1), BOX));

	/**/for (RigidParticle* rp : p)
		rp->setBoxSize(1, 5, 1);

	bolos.push_back(p);
}

SolidSystem::SolidSystem(PxScene* s, PxPhysics* p)
{
	cameraTrigger = new CameraTrigger(Vector3(0, 0, 0), 20, 50, 20, GetCamera());
	force = 100000;
	canThrow = true;
	ball = new RigidParticle(Vector3(0, 0, 0), Vector3(0, 0, 0), 1, p, s, 1000);
	ballTrigger = new BallTrigger(Vector3(0, 0, 70), 20, 50, 20, ball);
	scene = s;
	physics = p;

	generateBolos(Vector3(10, 5, 50));
}

SolidSystem::~SolidSystem()
{
	delete cameraTrigger;
	delete ballTrigger;
	delete ball;
	
	for (std::vector<RigidParticle*> v : bolos)
		for (RigidParticle* p : v)
			delete p;
}

void SolidSystem::update(double t)
{

	cameraTrigger->update();

	ballTrigger->update();
	if (ballTrigger->isInside()) {
		ball->getRigid()->setGlobalPose(PxTransform(Vector3(0, 0, 0)));
		canThrow = true;
		ball->getRigid()->setLinearVelocity({ 0, 0, 0 });
	}
}

void SolidSystem::keyPress(unsigned char key)
{
	switch (toupper(key))
	{
	case 'E': {
		if (cameraTrigger->isInside() && canThrow) {
			generateBall();
		}
		break;
	}
	case 'F': {
		std::cin >> force;
		break;
	}
	default:
		break;
	}
}
