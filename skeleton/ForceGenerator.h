#pragma once
#include "Particle.h"
class ForceGenerator {
public:
	virtual void updateForce(Particle* particle, double t) = 0;
	std::string _name;
	double _t = 0.0; // If starting negative --> eternal
	double _duration = -1e10;
	inline bool updateTime(double t) {
		_t += t;
		return _t < _duration || _duration < 0.0; // Devuelve true si ya era cero o si es positivo
	}
	virtual ~ForceGenerator() {}
};

class GravityForceGenerator : public ForceGenerator {
protected:
	Vector3 _gravity;
public:
	GravityForceGenerator(const Vector3& g) : _gravity(g) {};
	void updateForce(Particle* p, double t) override;
	inline void setGravity(Vector3 g) { _gravity = g; }
};

class WindGenerator : public ForceGenerator {
protected:
	Vector3 _windVelocity, _position;
	float k1 = 0, k2 = 0, radius;
public:
	WindGenerator(Vector3 p, Vector3 v, float r, float coef1 = 0, float coef2 = 0) : _position(p), _windVelocity(v), radius(r), k1(coef1), k2(coef2) {};
	void updateForce(Particle* p, double t) override;
	inline void setWindVelocity(Vector3 v) { _windVelocity = v; }
	inline void setK1(float coef1) { k1 = coef1; }
	inline void setK2(float coef2) { k2 = coef2; }
};

class TorbellinoGenerator : public WindGenerator {
protected:
	float K;
public:
	TorbellinoGenerator(Vector3 p, Vector3 v, float r, float force, float coef1 = 0, float coef2 = 0) : WindGenerator(p, v, r, coef1, coef2), K(force) {};
	void updateForce(Particle* p, double t) override;
};

class ExplosionGenerator : public ForceGenerator {
protected:
	Vector3 _position, _velocity;
	float K, radius, timeConst, time;
	bool active;
public:
	ExplosionGenerator(Vector3 p, Vector3 v, float force, float t = 1.0f) : _position(p), _velocity(v), K(force), radius(0), timeConst(t), time(0), active(false) {};
	void updateForce(Particle* p, double t);
	inline void setActive() { active = true; }
	inline bool getActive() { return active; }
	inline void updateTime(double t) { time += t; }
};


