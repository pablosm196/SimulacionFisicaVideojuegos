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
	std::vector<Bolo*> p;

	p.push_back(new Bolo(Vector3(0, 0, 0), pos + Vector3(0, 0, -5), 1, physics, scene, 100.0f, Vector4(1, 1, 1, 1), BOX));
	p.push_back(new Bolo(Vector3(0, 0, 0), pos + Vector3(2.5, 0, 0), 1, physics, scene, 100.0f, Vector4(1, 1, 1, 1), BOX));
	p.push_back(new Bolo(Vector3(0, 0, 0), pos + Vector3(-2.5, 0, 0), 1, physics, scene, 100.0f, Vector4(1, 1, 1, 1), BOX));
	p.push_back(new Bolo(Vector3(0, 0, 0), pos + Vector3(0, 0, 5), 1, physics, scene, 100.0f, Vector4(1, 1, 1, 1), BOX));
	p.push_back(new Bolo(Vector3(0, 0, 0), pos + Vector3(5, 0, 5), 1, physics, scene, 100.0f, Vector4(1, 1, 1, 1), BOX));
	p.push_back(new Bolo(Vector3(0, 0, 0), pos + Vector3(-5, 0, 5), 1, physics, scene, 100.0f, Vector4(1, 1, 1, 1), BOX));

	for (RigidParticle* rp : p) {
		rp->setBoxSize(1, 5, 1);
		rp->getRigid()->setMassSpaceInertiaTensor(Vector3(1, 7, 1));
	}

	bolos.push_back(p);
}

void SolidSystem::generateWalls(Vector3 pos)
{
	PxRigidStatic* wall1 = physics->createRigidStatic(PxTransform(pos + Vector3(15, 0, 0)));
	PxShape* pared = CreateShape(PxBoxGeometry(1, 20, 50));
	wall1->attachShape(*pared);
	scene->addActor(*wall1);
	RenderItem* wallItem1 = new RenderItem(pared, wall1, { 1, 0, 0, 1 });
	walls.push_back(wall1);

	Particle* rail1 = new Particle(Vector3(0, 0, 0), pos + Vector3(11, 1, 0), 0.998, 1, 5000, Vector4{ 0, 0, 1, 1 }, BOX);
	rail1->setBoxSize(5, 1, 50);
	rail1->setMass(rail1->getVolumen() * 1000);
	railes.push_back(rail1);
	BuoyancyForceGenerator* rail1Force = new BuoyancyForceGenerator(rail1, scene->getGravity().magnitude(), Vector3(0, 0, 1000));
	ForceRegistry->addRegistry(rail1Force, ball);
	forces.push_back(rail1Force);

	PxRigidStatic* wall2 = physics->createRigidStatic(PxTransform(pos + Vector3(-15, 0, 0)));
	wall2->attachShape(*pared);
	scene->addActor(*wall2);
	RenderItem* wallItem2 = new RenderItem(pared, wall2, { 1, 0, 0, 1 });
	walls.push_back(wall2);

	Particle* rail2 = new Particle(Vector3(0, 0, 0), pos + Vector3(-11, 1, 0), 0.998, 1, 5000, Vector4{ 0, 0, 1, 1 }, BOX);
	rail2->setBoxSize(5, 1, 50);
	rail2->setMass(rail1->getVolumen() * 1000);
	railes.push_back(rail2);
	BuoyancyForceGenerator* rail2Force = new BuoyancyForceGenerator(rail2, scene->getGravity().magnitude(), Vector3(0, 0, 1000));
	ForceRegistry->addRegistry(rail2Force, ball);
	forces.push_back(rail2Force);
}

SolidSystem::SolidSystem(PxScene* s, PxPhysics* p)
{
	cameraTrigger = new CameraTrigger(Vector3(0, 0, 0), 20, 20, 20, GetCamera());
	cameraTrigger2 = new CameraTrigger(Vector3(100, 0, 0), 20, 20, 20, GetCamera());
	force = 100000;
	canThrow = true;
	win = false;
	ball = new RigidParticle(Vector3(0, 0, 0), Vector3(0, 3, 0), 1, p, s, 1000);
	ballTrigger = new BallTrigger(Vector3(-5, 0, 70), 30, 50, 20, ball);
	ballTrigger2 = new BallTrigger(Vector3(95, 0, 70), 30, 50, 20, ball);
	scene = s;
	physics = p;
	ForceRegistry = new ParticleForceRegistry();

	WindGenerator* wg = new WindGenerator(Vector3(100, 0, 30), Vector3(2000, 0, 0), 15, 1, 0);
	wg->setActive(true);
	ForceRegistry->addRegistry(wg, ball);
	forces.push_back(wg);

	callback = new MyCallBacks();
	scene->setSimulationEventCallback(callback);

	generateBolos(Vector3(10, 6, 50));
	generateWalls(Vector3(10, 0, 45));

	generateBolos(Vector3(110, 6, 50));
	generateWalls(Vector3(110, 0, 45));
}

SolidSystem::~SolidSystem()
{
	delete cameraTrigger;
	delete cameraTrigger2;
	delete ballTrigger;
	delete ballTrigger2;
	scene->removeActor(*ball->getRigid());
	delete ForceRegistry;
	
	for (std::vector<Bolo*> v : bolos)
		for (RigidParticle* p : v)
			delete p;

	for (Particle* p : railes)
		delete p;

	for (ForceGenerator* f : forces)
		delete f;
}

void SolidSystem::update(double t)
{
	ForceRegistry->updateForces(t);
	cameraTrigger->update();
	cameraTrigger2->update();

	ballTrigger->update();
	ballTrigger2->update();
	if (ballTrigger->isInside() || ballTrigger2->isInside() || ball->getRigid()->getGlobalPose().p.y < 0) {
		ball->getRigid()->setGlobalPose(PxTransform(Vector3(0, 0, 0)));
		canThrow = true;
		ball->getRigid()->setLinearVelocity({ 0, 0, 0 });
	}

	bool sigue = true;
	int i = 0, j;
	
	while (i < bolos.size() && sigue) {
		j = 0;
		while (j < bolos[i].size() && bolos[i][j]->getTirado()) j++;
		if (j < bolos[i].size()) sigue = false;
		++i;
	}

	win = i == bolos.size() && sigue;
}

void SolidSystem::keyPress(unsigned char key)
{
	switch (toupper(key))
	{
	case 'E': {
		if ((cameraTrigger->isInside() || cameraTrigger2->isInside()) && canThrow) {
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
