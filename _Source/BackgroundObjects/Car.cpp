#include "stdafx.h"
#include "Car.h"

Car::Car()
	:BackgroundObject()
	, moveSpeed(500.f)
	, bMove(false), bDead(false)
{}

Car::~Car()
{
	SAFE_DELETE(animation)
}

void Car::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	animation->Update(V, P);
}

void Car::Render()
{
	animation->Render();
}

void Car::Position(float x, float y)
{
	animation->Position(x, y);
}

void Car::Position(D3DXVECTOR2 & vec)
{
	animation->Position(vec);
}

void Car::Scale(float x, float y)
{
	animation->Scale(x, y);
}

void Car::Scale(D3DXVECTOR2 & vec)
{
	animation->Scale(vec);
}



