#pragma once
#include<list>
#include "Particle.h"
#include"ParticleGenerator.h"
#include"Firework.h"
#include "ParticleForceRegistry.h"
#include "ForceGenerator.h"
class ParticleSystem
{
	const float NEW_PARTICLE_TIME = 0.1f;
	const float NEW_FIREWORK_TIME = 3.0f;
private:
	std::list<Particle*> _particles;
	std::list<Firework*> _fireworks;
	std::list<ParticleGenerator*> _particle_generators;
	std::list<ForceGenerator*> _forces;
	std::list<ForceGenerator*> _forcesSpring;
	FireworkGenerator* _firework_generator;
	GaussianRigidGenerator* rg;

	float newParticle, time, newFirework, timeFirework;

	ParticleForceRegistry* ForceRegistry;
	Particle* water;

	void generateSpringDemo();
public:
	ParticleSystem(PxScene* s, PxPhysics* p) : _particles(0), _particle_generators(0), newParticle(0.0f), time(0.0f), timeFirework(0.0f), newFirework(0.0f){
		/*UniformParticleGenerator* ug = new UniformParticleGenerator(Vector3(0, 0, 0), Vector3(0, 10, 0), 5);
		_particle_generators.push_back(ug);
		GaussianParticleGenerator* gg = new GaussianParticleGenerator(Vector3(0, 0, 0), Vector3(0, 0, 0), 1, 1);
		_particle_generators.push_back(gg);*/
		_firework_generator = new FireworkGenerator(Vector3(0, 0, 0), Vector3(0, 1, 0), Vector3(0, 0, 0), 1);
		rg = new GaussianRigidGenerator(Vector3(0, 15, 0), Vector3(0, 10, 0), 1, 1, 100, p, s);
		rg->setActive(true);

		ForceRegistry = new ParticleForceRegistry();

		GravityForceGenerator* Gravity = new GravityForceGenerator(Vector3(0, -9.8, 0));
		_forces.push_back(Gravity);
		TorbellinoGenerator* Torbellino = new TorbellinoGenerator(Vector3(0, 0, 0), Vector3(10, 0, 0), 20, 5, 4);
		_forces.push_back(Torbellino);
		WindGenerator* Wind = new WindGenerator(Vector3(0, 0, 0), Vector3(0, 0, 100), 100.0f, 1.0f);
		_forces.push_back(Wind);
		ExplosionGenerator* Explosion = new ExplosionGenerator(Vector3(0, 0, 0), Vector3(100, 100, 100), 5000, 2.0f);
		_forces.push_back(Explosion);

		//generateSpringDemo();
	};
	~ParticleSystem();
	void update(double t);
	ParticleGenerator* getParticleGenerator(std::string name);
	void generateFireworkSystem();
	void generateExplosion();
	void setGravity(bool g);
	void setTorbellino(bool t);
	void setWind(bool w);
	//inline void setKMuelle(float k) { muelle->setK(k); }
	inline void setFireworkGenerator(bool active) { _firework_generator->setActive(active); }
	inline void setRigidGenerator(bool active) { rg->setActive(active); }
};

