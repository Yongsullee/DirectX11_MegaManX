#include "stdafx.h"
#include "BigBridge.h"

constexpr float BigBridgeY = -350.f;

BigBridge::BigBridge()
	:Bridge()
{
	SetBPartiable(false);

	wstring textureFile = L"";
	wstring shaderFile = Shaders + L"009_Sprite.fx";

	textureFile = L"/MegaManX/Background/Bg.png";

	animation = new Animation;

	Clip* clip;
	
	//Idle
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(Textures + textureFile, Shaders + L"009_Sprite.fx", 4, 4, 4 + 7680, 4 + 463),0.3f);
		animation->AddClip(clip);
	}

	animation->Scale(3.0f, 3.0f);
	animation->Position(Width*0.5f, -350.f);
	animation->Play(0);
	animation->DrawBound(false);
}

BigBridge::~BigBridge()
{}

void BigBridge::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	__super::Update(V, P);
}

void BigBridge::Render()
{	
	__super::Render();
}
