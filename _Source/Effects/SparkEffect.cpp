#include "stdafx.h"
#include "Effects/SparkEffect.h"

SparkEffect::SparkEffect(D3DXVECTOR2 position)
	:Effect()
	, stateNumber(0), angle(0)
{
	wstring spriteFile = Textures + L"MegaManX/Effect/SparkEffect.png";
	wstring shaderFile = Shaders + L"009_Sprite.fx";

	animation = new Animation;

	Clip* clip;
	//idle
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 26, 74, 26 + 16, 74 + 8), 0.1f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 49, 71, 49 + 18, 71 + 11), 0.1f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 72, 72, 72 + 16, 72 + 10), 0.1f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 93, 74, 93 + 14, 74 + 8), 0.1f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 112, 75, 112 + 13, 75 + 7), 0.1f);
		animation->AddClip(clip);
	}

	animation->Scale(2.0f, 2.0f);
	animation->Position(position);
	animation->DrawBound(false);

	animation->Play(stateNumber);
}

SparkEffect::~SparkEffect()
{
	SAFE_DELETE(animation);
}

void SparkEffect::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	__super::Update(V, P);

	D3DXVECTOR2 position = playerPos;
		
	float xPos = PositionX;
	float yPos = PositionY;

	if (angle == 180)
	{
		xPos *= -1.0f;
	}

	playerPos.x += xPos;
	playerPos.y += yPos;

	animation->Position(playerPos);
	animation->Play(stateNumber);
	animation->Update(V, P);
}

void SparkEffect::Render()
{
	if (bRender == true)
	{
		__super::Render();
		animation->Render();
	}
}