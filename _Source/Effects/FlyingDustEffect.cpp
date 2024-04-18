#include "stdafx.h"
#include "Effects/FlyingDustEffect.h"

FlyingDustEffect::FlyingDustEffect(D3DXVECTOR2 position)
	:Effect()
	, stateNumber(0), frameCycle(0)
	, bDead(false)
	, weaponPos(0.f, 0.f)
{
	wstring spriteFile = Textures + L"MegaManX/Effect/CrashDustEffect.png";
	wstring shaderFile = Shaders + L"009_Sprite.fx";

	animation = new Animation;

	Clip* clip;
	//idle
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 8, 10, 8 + 8, 10 + 8), 0.5f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 21, 10, 21 + 7, 10 + 8), 0.5f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 33, 11, 33 + 6, 11 + 7), 0.5f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 62, 10, 62 + 8, 10 + 8), 0.5f);
		clip->AddFrame(new Sprite(spriteFile, shaderFile, 81, 11, 81 + 7, 11 + 7), 0.5f);
		animation->AddClip(clip);
	}

	animation->Scale(3.0f, 3.0f);
	animation->Position(position);
	animation->DrawBound(false);

	animation->Play(stateNumber);
}

FlyingDustEffect::~FlyingDustEffect()
{
	SAFE_DELETE(animation);
}

void FlyingDustEffect::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	__super::Update(V, P);

	AdjustPosition(bAdjustable);

	HandleStateNumber();

	animation->Play(stateNumber);
	animation->Update(V, P);
}

void FlyingDustEffect::Render()
{
	if (bRender == true)
	{
		__super::Render();
		animation->Render();
	}
}

/*****************************************************************/ // Handle

void FlyingDustEffect::HandleStateNumber()
{
	// 1. Sub Boss의 추락
	if (bAdjustable == true)
	{
		DelayDeadState();

		CheckDeadStateCondition();
	}
	else
	{

	}
}

/*****************************************************************/ // State

/*

	목적:
		모든 Frame이 온전히 재생된 후 지우기위한 Delay를 걸어줍니다
	설명:
		1. SetBRender(false)	-> 마지막 Frame까지 도달하면 화면에 나타내는것을 멈추기위함입니다.
		2. SetBDead(true)	-> Enemy 객체에게 해당 Effect를 지워도 된다는 Flag입니다.

*/
void FlyingDustEffect::DelayDeadState()
{
	if (BRender() == true
		&& animation->GetClip()->CurrentFrame() == 4)
	{
		frameCycle++;
	}
}

void FlyingDustEffect::CheckDeadStateCondition()
{
	if (frameCycle >= 1)
	{
		SetBDead(true);
		SetBRender(false);
	}
}

/*****************************************************************/ // Adjust Position

void FlyingDustEffect::AdjustPosition(bool _adjustable)
{
	D3DXVECTOR2 position = animation->Position();

	if (_adjustable == false)
	{
		position.x = weaponPos.x + 40.f;
		position.y = weaponPos.y;
	}
	else if (_adjustable == true)
	{
		position.x = enemyPos.x + offsetPositionX;
		position.y = enemyPos.y + offsetPositionY;
	}

	animation->Position(position);
}
