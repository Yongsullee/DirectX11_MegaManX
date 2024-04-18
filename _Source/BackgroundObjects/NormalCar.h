#pragma once
#include "BackgroundObjects/Car.h"

class NormalCar : public Car
{
public:
	NormalCar(float xPos, int carNum, bool bCollidable, bool bMovable, bool bDeadable);
	virtual ~NormalCar();

	virtual void Update(D3DXMATRIX& V, D3DXMATRIX& P);

	virtual void Render();

	void MoveLeft();

private:
	int clipNum;
};