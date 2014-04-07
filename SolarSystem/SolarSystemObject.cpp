#include "SolarSystemObject.h"

SSO::SolarSystemObject::SolarSystemObject(int apoapsis, int periapsis, 
	int a, float e, int speed, int radius, float isometricRatio, float sizeRatio, float speedRatio, float distanceRatio, float t0)
{
	SolarSystemObject(apoapsis, periapsis, a, e, speed, radius, t0);
	this->Settings(speedRatio, sizeRatio, distanceRatio, isometricRatio);
}

SSO::SolarSystemObject::SolarSystemObject(int apoapsis, int periapsis, 
	int a, float e, int speed, int radius, float t0)
{
	this->apoapsis = apoapsis;
	this->periapsis = periapsis;
	this->origA = this->a = a;
	this->origB = this->b = a*sqrt(1 - e*e);
	this->l = 3.141592653 * (3 * (a + b) - sqrt((3 * a + b)*(a + 3 * b)));
	this->origE = this->e = e;
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

int SSO::SolarSystemObject::GetA() const
{
	return this->a;
}

int SSO::SolarSystemObject::GetB() const
{
	return this->a*sqrt(1 - e*e);
}

int SSO::SolarSystemObject::GetRadius() const
{
	return this->projectionRadius;
}

float SSO::SolarSystemObject::GetSpeedRatio() const
{
	return this->speedRatio;
}

float SSO::SolarSystemObject::GetSizeRatio() const
{
	return this->sizeRatio;
}

float SSO::SolarSystemObject::GetDistanceRatio() const
{
	return this->distanceRatio;
}

float SSO::SolarSystemObject::GetIsometricRatio() const
{
	return this->isometricRatio;
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

void SSO::SolarSystemObject::SetIsometricRatio(float ratio)
{
	this->isometricRatio = ratio;
	this->e = this->origE + this->isometricRatio;
	if (this->e <= 0.0001)
		this->e = 0.01;
	if (this->e >= 0.9999)
		this->e = 0.8;
}

void SSO::SolarSystemObject::Settings(float speedRatio, float sizeRatio, float distanceRatio, float isometricRatio)
{
	this->SetSpeedRatio(speedRatio);
	this->SetSizeRatio(sizeRatio);
	this->SetDistanceRatio(distanceRatio);
	this->SetIsometricRatio(isometricRatio);
}

int SSO::SolarSystemObject::Recalc(float t)
{
		if (t >= 2 * 3.141592653 || t <= 0.0)
		{
			t = 0.000000001;
		}
		this->e = this->origE + this->isometricRatio;
		if (this->e <= 0.0001)
			this->e = 0.0001;
		if (this->e >= 0.9999)
			this->e = 0.9999;
		this->t = t;
		this->x = this->a * sin(t) * this->distanceRatio;
		this->y = this->a * sqrt(1 - e*e) *cos(t) - this->distanceRatio;
		this->projectionRadius = this->radius * this->sizeRatio *(((cos(t) / 4) + 0.75) - ((cos(t) / 4) + 0.75)*this->isometricRatio);
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
