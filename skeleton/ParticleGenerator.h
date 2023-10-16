#pragma once
#include<string>
#include <list>
#include <random>
#include "Particle.h"
class ParticleGenerator
{
protected:
	std::string _name;
	Vector3 _mean_pos, _mean_vel;
	double _generator_probability;
	int _num_particles;
	Particle* _model;
public:
	void setParticle(Particle* model);
	virtual std::list<Particle*> generateParticles() = 0;
};

class GaussianParticleGenerator : public ParticleGenerator
{
protected:
	Vector3 std_dev_pos, std_dev_vel;
	double std_dev_t;
public:
	GaussianParticleGenerator(Vector3 pos, Vector3 vel, double t);
	std::list<Particle*> generateParticles() override;
};

class UniformParticleGenerator : public ParticleGenerator {
private:
	Vector3 _vel_width, _pos_width;
public:
	UniformParticleGenerator(Vector3 pos, Vector3 vel);
	std::list<Particle*> generateParticles() override;
};

