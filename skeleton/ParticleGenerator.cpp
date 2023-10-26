#include "ParticleGenerator.h"

void ParticleGenerator::setParticle(Particle* model)
{
	_model = model;
}

GaussianParticleGenerator::GaussianParticleGenerator(Vector3 pos, Vector3 vel, double t, int n)
{
	srand(time(0));
	std_dev_pos = pos;
	std_dev_vel = vel;
	std_dev_t = t;

	_num_particles = n;
}

std::list<Particle*> GaussianParticleGenerator::generateParticles()
{
	srand(time(0));
	std::list<Particle*> particles;

	Vector3 pos, vel;
	float r, g, b;
	for (int i = 0; i < _num_particles; ++i) {
		pos.x = std_dev_pos.x + dist(dre) * 10;
		pos.y = std_dev_pos.y + dist(dre) * 10;
		pos.z = std_dev_pos.z + dist(dre) * 10;

		vel.x = std_dev_vel.x + dist(dre) * 10;
		vel.y = std_dev_vel.y + dist(dre);
		vel.z = std_dev_vel.z + dist(dre) * 10;

		r = rand() % 255 / 255.0f;
		g = rand() % 255 / 255.0f;
		b = rand() % 255 / 255.0f;

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
	float r, g, b;
	for (int i = 0; i < _num_particles; ++i) {
		pos.x = _pos_width.x + dist(dre) * 10;
		pos.y = _pos_width.y + dist(dre) * 10;
		pos.z = _pos_width.z + dist(dre) * 10;

		vel.x = _vel_width.x + dist(dre) * 10;
		vel.y = _vel_width.y + dist(dre);
		vel.z = _vel_width.z + dist(dre) * 10;

		r = rand() % 255 / 255.0f;
		g = rand() % 255 / 255.0f;
		b = rand() % 255 / 255.0f;

		Particle* newParticle = new Particle(vel, pos, Vector3(0, -9.8f, 0), 0.998, 2.0f, Vector4(r, g, b, 1));
		particles.push_back(newParticle);
	}

	return particles;
}

std::list<Particle*> FireworkGenerator::generateRandom(Firework* parent)
{
	std::list<Particle*> particles;
	float r, g, b;
	Vector3 vel;
	for (int i = 0; i < parent->getNumHijos(); ++i) {
		vel.x = parent->getVel().x + dist(dre) * 10;
		vel.y = parent->getVel().y + dist(dre);
		vel.z = parent->getVel().z + dist(dre) * 10;

		r = rand() % 255 / 255.0f;
		g = rand() % 255 / 255.0f;
		b = rand() % 255 / 255.0f;
		particles.push_back(new Firework(vel, parent->getPos(), _mean_ac, 0.998f, parent->getType(), 2.0f, Vector4(r, g, b, 1), 2));
	}
	return particles;
}

std::list<Particle*> FireworkGenerator::generateCircle(Firework* parent)
{
	std::list<Particle*> list;
	float r = 10.0f, angle;
	Vector3 pos;
	for (int i = 0; i < parent->getNumHijos(); ++i) {
		angle = i * 360.0f / parent->getNumHijos() * 3.14f / 180;
		pos = parent->getPos() + Vector3(r * cos(angle), r * sin(angle), 0);
		list.push_back(new Firework(Vector3(0, 0, 0), pos, _mean_ac, 0.998f, Firework::CIRCLE, 2.0f, parent->getColor(), parent->getNumHijos()));
	}
	return list;
}

FireworkGenerator::FireworkGenerator(Vector3 pos, Vector3 vel, Vector3 ac, int n)
{
	srand(time(0));
	_mean_pos = pos;
	_mean_vel = vel;
	_mean_ac = ac;
	_num_particles = n;
}

std::list<Particle*> FireworkGenerator::generateParticles()
{
	std::list<Particle*> fireworks;
	float r, g, b;
	Vector3 pos, vel;
	for (int i = 0; i < _num_particles; ++i) {

		pos.x = _mean_pos.x + dist(dre) * 10;
		pos.y = _mean_pos.y + dist(dre) * 10;
		pos.z = _mean_pos.z + dist(dre) * 10;

		vel.x = _mean_vel.x + dist(dre) * 10;
		vel.y = _mean_vel.y + dist(dre);
		vel.z = _mean_vel.z + dist(dre) * 10;

		r = rand() % 255 / 255.0f;
		g = rand() % 255 / 255.0f;
		b = rand() % 255 / 255.0f;
		fireworks.push_back(new Firework(vel, pos, _mean_ac, 0.998f, Firework::CIRCLE, 2.0f, Vector4(r, g, b, 1), 5));
	}
	return fireworks;
}

std::list<Particle*> FireworkGenerator::generateParticlesFromFireworks(Firework* parent)
{
	std::list<Particle*> list;
	switch (parent->getType()) {
	case Firework::RANDOM:
		list =  generateRandom(parent); break;
	case Firework::CIRCLE:
		list = generateCircle(parent); break;
	default: break;
	}
	return list;
}

