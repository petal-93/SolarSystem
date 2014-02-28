#include "SolarSystemObject.h"

SSO::SolarSystemObject::SolarSystemObject(int aphelion, int perihelion, int a, float e, int speed, int radius, bool isometric, float sizeRatio, float speedRatio, float t0)
{
	this->aphelion = aphelion;
	this->perihelion = perihelion;
	this->a = a;
	this->b = a*sqrt(1 - e*e);
	this->l = 3.141592653 * (3 * (a + b) - sqrt((3 * a + b)*(a + 3 * b)));
	this->e = e;
	this->speed = speed;
	this->radius = radius;
	this->isometric = isometric;
	this->t = t0;
	this->Recalc(t0);
	this->sizeRatio = sizeRatio;
	this->speedRatio = speedRatio;
}


int SSO::SolarSystemObject::getX() const
{
	return this->x;
}

int SSO::SolarSystemObject::getY() const
{
	return this->y;
}

int SSO::SolarSystemObject::getRadius() const
{
	return this->projectionRadius;
}

float SSO::SolarSystemObject::getSpeedRatio() const
{
	return this->speedRatio;
}

void SSO::SolarSystemObject::setSizeRatio(float ratio)
{
	this->sizeRatio = ratio;
}

void SSO::SolarSystemObject::setSpeedRatio(float ratio)
{
	this->speedRatio = ratio;
}

int SSO::SolarSystemObject::Recalc(float t)
{
		if (t >= 2 * 3.141592653 || t <= 0.0)
		{
			t = 0.000000001;
		}
		this->t = t;
		this->x = this->a * sin(t);
		this->y = this->b * cos(t);

		if (this->isometric)
		{
			this->projectionRadius = this->radius * (cos(t)/3.5 + 0.75) * this->sizeRatio;
		}
		else
			this->projectionRadius = this->radius * this->sizeRatio;
		return 0;
}

float SSO::SolarSystemObject::Next()
{
	this->t += 0.0001 * this->speed  * this->speedRatio;
	if (t >= 2 * 3.141592653)
		t = 0.0001 * this->speed  * this->speedRatio;
	Recalc(t);
	return t;
}

SSO::SolarSystemObject::~SolarSystemObject()
{
}
