#include "SolarSystemObject.h"

SSO::SolarSystemObject::SolarSystemObject(int apoapsis, int periapsis, 
	int a, float e, int speed, int radius, bool isometric, float sizeRatio, float speedRatio, float distanceRatio, float t0)
{
	SolarSystemObject(apoapsis, periapsis, a, e, speed, radius, t0);
	this->Settings(speedRatio, sizeRatio, distanceRatio, isometric);
}

SSO::SolarSystemObject::SolarSystemObject(int apoapsis, int periapsis, 
	int a, float e, int speed, int radius, float t0)
{
	this->apoapsis = apoapsis;
	this->periapsis = periapsis;
	this->a = a;
	this->b = a*sqrt(1 - e*e);
	this->l = 3.141592653 * (3 * (a + b) - sqrt((3 * a + b)*(a + 3 * b)));
	this->e = e;
	this->speed = speed;
	this->radius = radius;
	this->t = t0;
	this->Recalc(t0);
	/*
	this->isometric = true;
	this->sizeRatio = 100;
	this->speedRatio = 100;*/
}

int SSO::SolarSystemObject::GetX() const
{
	return this->x;
}

int SSO::SolarSystemObject::GetY() const
{
	return this->y;
}

int SSO::SolarSystemObject::GetRadius() const
{
	return this->projectionRadius;
}

float SSO::SolarSystemObject::GetSpeedRatio() const
{
	return this->speedRatio;
}

void SSO::SolarSystemObject::SetSizeRatio(float ratio)
{
	this->sizeRatio = ratio;
}

void SSO::SolarSystemObject::SetSpeedRatio(float ratio)
{
	this->speedRatio = ratio;
}

void SSO::SolarSystemObject::SetDistanceRatio(float ratio)
{
	this->distanceRatio = ratio;
}

void SSO::SolarSystemObject::SetIsometric(bool isometric)
{
	this->isometric = isometric;
}

void SSO::SolarSystemObject::Settings(float speedRatio, float sizeRatio, float distanceRatio, bool isometric)
{
	this->SetSpeedRatio(speedRatio);
	this->SetSizeRatio(sizeRatio);
	this->SetDistanceRatio(distanceRatio);
	this->SetIsometric(isometric);
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
