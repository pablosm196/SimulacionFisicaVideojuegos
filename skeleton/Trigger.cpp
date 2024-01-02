#include "Trigger.h"

void Trigger::generateTrigger()
{
	v.push_back(new Particle({ 0, 0, 0 }, position, 0.998, 1));
	v.push_back(new Particle({ 0, 0, 0 }, position + Vector3(size.x, 0, 0), 0.998, 1));
	v.push_back(new Particle({ 0, 0, 0 }, position + Vector3(0, size.y, 0), 0.998, 1));
	v.push_back(new Particle({ 0, 0, 0 }, position + Vector3(0, 0, size.z), 0.998, 1));
	v.push_back(new Particle({ 0, 0, 0 }, position + Vector3(size.x, size.y, 0), 0.998, 1));
	v.push_back(new Particle({ 0, 0, 0 }, position + Vector3(size.x, 0, size.z), 0.998, 1));
	v.push_back(new Particle({ 0, 0, 0 }, position + Vector3(0, size.y, size.z), 0.998, 1));
	v.push_back(new Particle({ 0, 0, 0 }, position + size, 0.998, 1));
}

Trigger::Trigger(Vector3 pos, float x, float y, float z)
{
	position = pos;
	size = Vector3(x, y, z);
	inside = false;
	generateTrigger();
}

Trigger::~Trigger()
{
	for (int i = 0; i < v.size(); ++i)
		delete v[i];
}

void CameraTrigger::update()
{
	float x = cam->getTransform().p.x, y = cam->getTransform().p.y, z = cam->getTransform().p.z;
	inside = x > position.x && x < position.x + size.x && y > position.y && y < position.y + size.y && z > position.z && z < position.z + size.z;
}

void BallTrigger::update()
{
	if (ball == nullptr)
		return;
	float x = ball->getRigid()->getGlobalPose().p.x, y = ball->getRigid()->getGlobalPose().p.y, z = ball->getRigid()->getGlobalPose().p.z;
	inside = x > position.x && x < position.x + size.x && y > position.y && y < position.y + size.y && z > position.z && z < position.z + size.z;
}
