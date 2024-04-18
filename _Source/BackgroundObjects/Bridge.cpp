#include "stdafx.h"
#include "Bridge.h"

Bridge::Bridge()
	:BackgroundObject()
	, stateNumber(0)
	, bPartiable(false)
	, bBroken(false)
	, bFall(false)
{
	SetBCollide(true);
}

Bridge::~Bridge()
{
	SAFE_DELETE(animation);
}

void Bridge::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	animation->Update(V, P);
}

void Bridge::Render()
{
	animation->Render();
}

void Bridge::Position(float x, float y)
{
	animation->Position(x,y);
}

void Bridge::Position(D3DXVECTOR2 & vec)
{
	animation->Position(vec);
}

void Bridge::Scale(float x, float y)
{
	animation->Scale(x, y);
}

void Bridge::Scale(D3DXVECTOR2 & vec)
{
	animation->Scale(vec);
}

