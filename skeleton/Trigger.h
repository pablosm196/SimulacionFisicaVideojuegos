#pragma once
#include "Particle.h"
#include<vector>
class Trigger
{
protected:
	Vector3 position;
	Vector3 size;
	bool inside;
	std::vector<Particle*> v;
	void generateTrigger();
public:
	Trigger(Vector3 pos, float x, float y, float z);
	virtual ~Trigger();
	virtual void update() = 0;
	bool isInside() { return inside; }
};

class CameraTrigger : public Trigger {
protected:
	Camera* cam;
public:
	CameraTrigger(Vector3 pos, float x, float y, float z, Camera* camera) : Trigger(pos, x, y, z), cam(camera) {};
	~CameraTrigger() override = default;
	void update() override;
};

class BallTrigger : public Trigger {
protected:
	RigidParticle* ball;
public:
	BallTrigger(Vector3 pos, float x, float y, float z, RigidParticle* b) : Trigger(pos, x, y, z), ball(b) {};
	~BallTrigger() override = default;
	inline void setBall(RigidParticle* b) { ball = b; }
	void update() override;
};

