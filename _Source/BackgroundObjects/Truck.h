#pragma once
#include "Car.h"

class Truck : public Car
{
public:
	Truck(float xPos, int truckNum, bool bCollidable, bool bMovable, bool bDeadable);
	virtual ~Truck();

	virtual void Update(D3DXMATRIX& V, D3DXMATRIX& P);

	virtual void Render();

	void MoveLeft();

private:
	int clipNum;
};