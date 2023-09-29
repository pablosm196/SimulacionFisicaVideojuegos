#include "Projectile.h"

void Projectile::escalate(Vector3 rv, Vector3 fv, Vector3 a, float m)
{
	float c = 1.0f;
	if(fv.magnitude() != 0)
		c = rv.magnitude() / fv.magnitude();

	Imass = m * powf(c, 2);
	acceleration = Vector3(a.x, a.y * powf((1 / c), 2), a.z);
}
