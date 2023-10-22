#include "ParticleGenerator.h"

void ParticleGenerator::setParticle(Particle* model)
{
	_model = model;
}

GaussianParticleGenerator::GaussianParticleGenerator(Vector3 pos, Vector3 vel, double t, int n)
{
	std_dev_pos = pos;
	std_dev_vel = vel;
	std_dev_t = t;

	_num_particles = n;
}

std::list<Particle*> GaussianParticleGenerator::generateParticles()
{
	std::list<Particle*> particles;

	Vector3 pos, vel;
	int r, g, b;
	for (int i = 0; i < _num_particles; ++i) {
		pos.x = std_dev_pos.x + dist(dre) * 10;
		pos.y = std_dev_pos.y + dist(dre) * 10;
		pos.z = std_dev_pos.z + dist(dre) * 10;

		vel.x = std_dev_vel.x + dist(dre) * 10;
		vel.y = std_dev_vel.y + dist(dre);
		vel.z = std_dev_vel.z + dist(dre) * 10;

		r = rand() % 255 + 1;
		g = rand() % 255 + 1;
		b = rand() % 255 + 1;

		Particle* newParticle = new Particle(vel, pos, Vector3(0, -9.8f, 0), 0.998, 2.0f, Vector4(r, g, b, 1));
		particles.push_back(newParticle);
	}

	return particles;
}

UniformParticleGenerator::UniformParticleGenerator(Vector3 pos, Vector3 vel, int n)
{
	_pos_width = pos;
	_vel_width = vel;

	_num_particles = n;
}

std::list<Particle*> UniformParticleGenerator::generateParticles()
{
	std::list<Particle*> particles;

	Vector3 pos, vel;
	int r, g, b;
	for (int i = 0; i < _num_particles; ++i) {
		pos.x = _pos_width.x + dist(dre) * 10;
		pos.y = _pos_width.y + dist(dre) * 10;
		pos.z = _pos_width.z + dist(dre) * 10;

		vel.x = _vel_width.x + dist(dre) * 10;
		vel.y = _vel_width.y + dist(dre);
		vel.z = _vel_width.z + dist(dre) * 10;

		r = rand() % 255 + 1;
		g = rand() % 255 + 1;
		b = rand() % 255 + 1;

		Particle* newParticle = new Particle(vel, pos, Vector3(0, -9.8f, 0), 0.998, 2.0f, Vector4(r, g, b, 1));
		particles.push_back(newParticle);
	}

	return particles;
}
