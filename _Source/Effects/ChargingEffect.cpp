#include "stdafx.h"
#include "ChargingEffect.h"

ChargingEffect::ChargingEffect(D3DXVECTOR2 position)
	:Effect()
	, stateNumber(0)
{
	wstring spriteFile = Textures + L"MegaManX/Effect/Charging.png";
	wstring shaderFile = Shaders + L"009_Sprite.fx";

	animation = new Animation;

	Clip* clip;
	//idle
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 107, 118, 107 + 90, 118 + 90), 0.02f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 206, 121, 206 + 86, 121 + 86), 0.02f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 301, 121, 301 + 84, 121 + 84), 0.02f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 397, 122, 397 + 82, 122 + 82), 0.02f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 492, 125, 492 + 74, 125 + 74), 0.02f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 577, 134, 577 + 64, 134 + 66), 0.02f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 656, 148, 656 + 45, 148 + 44), 0.02f);
		animation->AddClip(clip);
	} 

	animation->Scale(2.5f, 2.5f);
	animation->Position(position);
	animation->DrawBound(false);

	animation->Play(stateNumber);
}

ChargingEffect::~ChargingEffect()
{
	SAFE_DELETE(animation);
}

void ChargingEffect::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	__super::Update(V, P);

	animation->Position(playerPos);
	animation->Play(stateNumber);
	animation->Update(V, P);
}

void ChargingEffect::Render()
{
	if (bRender == true)
	{
		__super::Render();
		animation->Render();
	}
}