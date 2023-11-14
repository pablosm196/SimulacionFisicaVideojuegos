#pragma once
#include "Particle.h"
class ForceGenerator {
public:
	virtual void updateForce(Particle* particle) = 0;
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
	void updateForce(Particle* p);
	inline void setGravity(Vector3 g) { _gravity = g; }
};


