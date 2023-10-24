#pragma once
#include<list>
#include "Particle.h"
#include"ParticleGenerator.h"
#include"Firework.h"
class ParticleSystem
{
	const float NEW_PARTICLE_TIME = 2.0f;
	const float NEW_FIREWORK_TIME = 3.0f;
private:
	std::list<Particle*> _particles;
	std::list<Firework*> _fireworks;
	std::list<ParticleGenerator*> _particle_generators;
	FireworkGenerator* _firework_generator;
	float newParticle, time, newFirework, timeFirework;
public:
	ParticleSystem() : _particles(0), _particle_generators(0), newParticle(0.0f), time(0.0f), timeFirework(0.0f), newFirework(0.0f) {
		/*UniformParticleGenerator* ug = new UniformParticleGenerator(Vector3(0, 0, 0), Vector3(0, 0, 0), 1);
		_particle_generators.push_back(ug);*/
		/*GaussianParticleGenerator* gg = new GaussianParticleGenerator(Vector3(0, 0, 0), Vector3(0, 0, 0), 1, 1);
		_particle_generators.push_back(gg);*/
		_firework_generator = new FireworkGenerator(Vector3(0, 0, 0), Vector3(0, 1, 0), Vector3(0, 0, 0), 1);
	};
	~ParticleSystem();
	void update(double t);
	ParticleGenerator* getParticleGenerator(std::string name);
	void generateFireworkSystem();
};

