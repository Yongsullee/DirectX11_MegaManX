#pragma once
#include "BackgroundObject.h"

class StreetLamp : public BackgroundObject
{
public:
	StreetLamp();
	~StreetLamp();

	virtual void Update(D3DXMATRIX& V, D3DXMATRIX& P);

	virtual void Render();

private:
	vector<Sprite*> sprites;

private:
	const float POS_Y = 148.f;

	const float POS_1_X = -9643.f;
	const float POS_2_X = -9000.f;
};