#include "stdafx.h"
#include "Marker.h"

Marker::Marker(wstring shaderFile, D3DXVECTOR2 start)
	:scale(1,1), rotation(0,0,0)
{
	clip = new Clip(PlayMode::Loop);
	for (UINT i = 0; i < 7; i++)
	{
		float x = (float)i * 15 + 173;
		clip->AddFrame(new Sprite(Textures + L"Mario/Bullets.png", shaderFile, x, 155, x + 11, 167), 0.3f);
	}

	position = start;
	clip->Position(position);
	clip->Play();

}

Marker::~Marker()
{
	SAFE_DELETE(clip);
}

void Marker::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{

	clip->Position(position);
	clip->Update(V, P);
}

void Marker::Render()
{
	clip->Render();
}
