#pragma once
#include<string>
#include <list>
#include <random>
#include "Particle.h"
#include "Firework.h"
class ParticleGenerator
{
protected:
	std::string _name;
	Vector3 _mean_pos, _mean_vel;
	double _generator_probability;
	int _num_particles;
	Particle* _model;
	std::default_random_engine dre;
public:
	void setParticle(Particle* model);
	virtual std::list<Particle*> generateParticles() = 0;
};

class GaussianParticleGenerator : public ParticleGenerator
{
protected:
	Vector3 std_dev_pos, std_dev_vel;
	double std_dev_t;
	std::normal_distribution<> dist{ 0, 0.5 };
public:
	GaussianParticleGenerator(Vector3 pos, Vector3 vel, double t, int n);
	std::list<Particle*> generateParticles() override;
};

class UniformParticleGenerator : public ParticleGenerator {
private:
	Vector3 _vel_width, _pos_width;
	std::uniform_real_distribution<> dist{ -1, 1 };
public:
	UniformParticleGenerator(Vector3 pos, Vector3 vel, int n);
	std::list<Particle*> generateParticles() override;
};

class FireworkGenerator : public ParticleGenerator {
private:
	Vector3 _mean_ac;
	std::normal_distribution<> dist{ -1, 1 };
public:
	FireworkGenerator(Vector3 pos, Vector3 vel, Vector3 ac, int n);
	std::list<Particle*> generateParticles() override;
	std::list<Particle*> generateParticlesFromFireworks(Firework* parent);
};

