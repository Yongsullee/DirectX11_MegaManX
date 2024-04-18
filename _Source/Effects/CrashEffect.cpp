#include "stdafx.h"
#include "Effects/CrashEffect.h"

CrashEffect::CrashEffect(D3DXVECTOR2 position)
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
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 104, 69, 104 + 16, 69 + 16), 0.05f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 127, 61, 127 + 32, 61 + 32), 0.05f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 169, 64, 169 + 28, 64 + 24), 0.05f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 213, 62, 213 + 30, 62 + 27), 0.3f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 256, 63, 256 + 32, 63 + 27), 0.2f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 296, 67, 296 + 32, 67 + 23), 0.15f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 344, 71, 344 + 32, 71 + 16), 0.1f);
		animation->AddClip(clip);
	}

	animation->Scale(2.5f, 2.5f);
	animation->Position(position);
	animation->DrawBound(false);

	animation->Play(stateNumber);
}

CrashEffect::~CrashEffect()
{
	SAFE_DELETE(animation);
}

void CrashEffect::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	__super::Update(V, P);

	HandleStateNumber();

	AdjustPosition(bAdjustable);

	animation->Play(stateNumber);
	animation->Update(V, P);
}

void CrashEffect::Render()
{
	if (bRender == true)
	{
		__super::Render();
		animation->Render();
	}
}

/*****************************************************************/ // Handle

void CrashEffect::HandleStateNumber()
{
	DelayDeadState();

	CheckDeadStateCondition();
}

/*****************************************************************/ // State

/*
	목적:
		모든 Frame이 온전히 재생된 후 지우기위한 Delay를 걸어줍니다
	설명:
		1. SetBRender(false)	-> 마지막 Frame까지 도달하면 화면에 나타내는것을 멈추기위함입니다.
		2. SetBDead(true)	-> Enemy 객체에게 해당 Effect를 지워도 된다는 Flag입니다.
*/

void CrashEffect::DelayDeadState()
{

	if (BRender() == true
		&& animation->GetClip()->CurrentFrame() == 6)
	{
		frameCycle++;
	}
}

void CrashEffect::CheckDeadStateCondition()
{
	if (frameCycle >= 1)
	{
		SetBDead(true);
		SetBRender(false);
	}
}

/*****************************************************************/ // Adjust Position

void CrashEffect::AdjustPosition(bool _adjustable)
{
	D3DXVECTOR2 position = animation->Position();

	if (_adjustable == false)
	{
		if (enemyPos.x != 0)
		{
			position.x = enemyPos.x + positionX;
			position.y = enemyPos.y + 20.f;
		}
		else
		{
			position = weaponPos;
		}
	}else if (_adjustable == true)
	{
		position.x = enemyPos.x + offsetPositionX;
		position.y = enemyPos.y + offsetPositionY;
	}

	animation->Position(position);
}
