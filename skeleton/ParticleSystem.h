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
	FireworkGenerator* _firework_generator;
	float newParticle, time, newFirework, timeFirework;
	ParticleForceRegistry* ForceRegistry;
	GravityForceGenerator* Gravity;
	TorbellinoGenerator* Torbellino;
	WindGenerator* Wind;
	ExplosionGenerator* Explosion;

	void generateSpringDemo();
public:
	ParticleSystem() : _particles(0), _particle_generators(0), newParticle(0.0f), time(0.0f), timeFirework(0.0f), newFirework(0.0f) {
		/**/UniformParticleGenerator* ug = new UniformParticleGenerator(Vector3(0, 0, 0), Vector3(0, 10, 0), 5);
		_particle_generators.push_back(ug);
		/**/GaussianParticleGenerator* gg = new GaussianParticleGenerator(Vector3(0, 0, 0), Vector3(0, 0, 0), 1, 1);
		_particle_generators.push_back(gg);
		_firework_generator = new FireworkGenerator(Vector3(0, 0, 0), Vector3(0, 1, 0), Vector3(0, 0, 0), 1);

		//ug->setActive(true);

		ForceRegistry = new ParticleForceRegistry();
		Gravity = new GravityForceGenerator(Vector3(0, -9.8, 0));
		Torbellino = new TorbellinoGenerator(Vector3(0, 0, 0), Vector3(10, 0, 0), 20, 5, 4);
		Wind = new WindGenerator(Vector3(0, 0, 0), Vector3(-100, 0, 70), 10.0f, 0.0f);
		Explosion = new ExplosionGenerator(Vector3(0, 0, 0), Vector3(100, 100, 100), 5000, 2.0f);

		generateSpringDemo();
	};
	~ParticleSystem();
	void update(double t);
	ParticleGenerator* getParticleGenerator(std::string name);
	void generateFireworkSystem();
	void generateExplosion();
	void setGravity(bool g);
	void setTorbellino(bool t);
	void setWind(bool w);
};

