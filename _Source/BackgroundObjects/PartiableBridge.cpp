#include "stdafx.h"
#include "PartiableBridge.h"
#include "Enemies/Enemy.h"
#include "Enemies/SubBossEnemy.h"

constexpr float FIRST_PARTIABLE_BRIDGE_POS_X = -3105.f;
constexpr float FIRST_PARTIABLE_BRIDGE_POS_Y = -129.f;

constexpr float SECOND_PARTIABLE_BRIDGE_POS_X = -1185.f;
constexpr float SECOND_PARTIABLE_BRIDGE_POS_Y = -224.f;

PartiableBridge::PartiableBridge(int num)
	:Bridge()
	, positionNumber(num)
{
	SetBCollide(false);
	SetBPartiable(true);

	wstring textureFile = L"";
	wstring shaderFile = Shaders + L"009_Sprite.fx";

	textureFile = L"/MegaManX/Background/Bg2.png";

	animation = new Animation;

	Clip* clip;

	//Normal
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(Textures + textureFile, Shaders + L"009_Sprite.fx", 2192, 262, 2192 + 288, 262 + 109), 0.3f);
		animation->AddClip(clip);
	}
	//Broken
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(Textures + textureFile, Shaders + L"009_Sprite.fx", 2225, 383, 2225 + 222, 383 + 48), 0.3f);
		animation->AddClip(clip);
	}

	animation->Scale(3.0f, 3.0f);
	switch (positionNumber)
	{
	case 1:
		animation->Position(FIRST_PARTIABLE_BRIDGE_POS_X, FIRST_PARTIABLE_BRIDGE_POS_Y);
		break;
	case 2:
		animation->Position(SECOND_PARTIABLE_BRIDGE_POS_X, SECOND_PARTIABLE_BRIDGE_POS_Y);
		break;
	}
	animation->Play(StateNumber());
	animation->DrawBound(false);
}

PartiableBridge::~PartiableBridge()
{}

void PartiableBridge::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	__super::Update(V, P);

	HandleStateNumber();
	HandleMovement();

	CheckSubBossCollision();

	animation->Play(StateNumber());
}

void PartiableBridge::Render()
{
	__super::Render();
}

/***************************************/ //Handle

void PartiableBridge::HandleStateNumber()
{
	// 0 to 1
	CheckStateNumberChange0to1Condition();
}

void PartiableBridge::HandleMovement()
{
	D3DXVECTOR2 position = animation->Position();

	// 추락 조건
	if (BBroken() == true)
	{
		Fall(position);
	}

	animation->Position(position);
}

/***************************************/ //State

void PartiableBridge::CheckStateNumberChange0to1Condition()
{
	if (BBroken() == true)
		SetStateNumber(1);
}

/***************************************/ //Collision

void PartiableBridge::CheckSubBossCollision()
{
	if (BCollideWithSubBoss() == true)
	{
		SetBBroken(true);
	}
}

/***************************************/ //Movement

void  PartiableBridge::Fall(D3DXVECTOR2& _pos)
{
	velocity += gravity * Timer->Elapsed();

	_pos.y += velocity;

	// 착지 조건 
	if (_pos.y <= GroundPosition())
	{
		_pos.y = GroundPosition();
	}

}
