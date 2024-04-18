#include "stdafx.h"
#include "DustEffect.h"

DustEffect::DustEffect(D3DXVECTOR2 position)
	:Effect()
	, stateNumber(0), frameCycle(0)
	, bDead(false)
{
	wstring spriteFile = Textures + L"MegaManX/Effect/CrashDustEffect.png";
	wstring shaderFile = Shaders + L"009_Sprite.fx";

	animation = new Animation;

	Clip* clip;

	//idle
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 41, 35, 41+8, 35+8), 0.15f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 53, 34, 53+10, 34+11), 0.25f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 70, 33, 70+13, 33+13), 0.3f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 90, 33, 90+14, 33+14), 0.1f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 111, 33, 111+14, 33+14), 0.05f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 132, 39, 132+12, 39+6), 0.02f);
		animation->AddClip(clip);
	}

	animation->Scale(3.5f, 3.5f);
	animation->Position(position);
	animation->DrawBound(false);
	animation->Play(stateNumber);
}

DustEffect::~DustEffect()
{
	SAFE_DELETE(animation);
}

void DustEffect::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	__super::Update(V, P);

	HandleStateNumber();

	AdjustPosition(bAdjustable);

	animation->Play(stateNumber);
	animation->Update(V, P);
}

void DustEffect::Render()
{
	if (BRender() == true)
	{
		__super::Render();
		animation->Render();
	}
}

/*****************************************************************/ // Handle

void DustEffect::HandleStateNumber()
{
	DelayDeadState();

	CheckDeadStateCondition();
}

/*****************************************************************/ // State

void DustEffect::DelayDeadState()
{
	if (BRender() == true
		&& animation->GetClip()->CurrentFrame() == 5)
	{
		frameCycle++;
	}
}

void DustEffect::CheckDeadStateCondition()
{
	if (frameCycle >= 1)
	{
		SetBDead(true);
		SetBRender(false);
	}
}

/*****************************************************************/ // Adjust Position

void DustEffect::AdjustPosition(bool _adjustable)
{
	D3DXVECTOR2 position = animation->Position();

	if (_adjustable == false)
	{
		position = enemyPos;
	}
	else
	{
		position.x = enemyPos.x + offsetPositionX;
		position.y = enemyPos.y + offsetPositionY;
	}

	animation->Position(position);
}
