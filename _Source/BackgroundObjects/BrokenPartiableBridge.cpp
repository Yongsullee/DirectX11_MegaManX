#include "stdafx.h"
#include "BrokenPartiableBridge.h"
#include "Effects/DustEffect.h"

BrokenPartiableBridge::BrokenPartiableBridge(D3DXVECTOR2 _pos)
	:Bridge()
{
	SetBCollide(false);
	SetBPartiable(false);

	wstring textureFile = Textures + L"/MegaManX/Background/Bg2.png";
	wstring shaderFile = Shaders + L"009_Sprite.fx";

	animation = new Animation;

	Clip* clip;

	// Idle
	{
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(textureFile, shaderFile, 2149, 381, 2149+32, 381+56), 0.3f);
		animation->AddClip(clip);
	}

	animation->Scale(3.0f, 3.0f);
	animation->Position(_pos);
	animation->Play(StateNumber());
	animation->DrawBound(false);

	/***************************************/ //Effect

	DustEffect* effect;

	for (int i = 0; i < 4; i++)
	{
		effect = new DustEffect(_pos);
		effect->SetBAdjustable(true);
		dustEffects.push_back(effect);
	}

}

BrokenPartiableBridge::~BrokenPartiableBridge()
{
	for (DustEffect* effect : dustEffects)
		SAFE_DELETE(effect);
}

void BrokenPartiableBridge::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	D3DXVECTOR2 position = animation->Position();

	__super::Update(V, P);

	HandleMovement();

	animation->Play(StateNumber());

	/***************************************/ //Effect

	if (BRender() == true)
	{
		for (DustEffect* effect : dustEffects)
		{
			effect->GetEnemyPos(position);

			// X offset
			if (SmallPartiableBridgeSize() == 0)
				effect->SetOffsetPositionX(effectOffsetX_0);
			else
				effect->SetOffsetPositionX(effectOffsetX_1);

			// Y offset
			effect->SetOffsetPositionY(Math::Random(-65.f, 65.f));

			effect->Update(V, P);
		}
	}

}

void BrokenPartiableBridge::Render()
{
	if (BRender() == true)
	{
		__super::Render();
	}

	for (DustEffect* effect : dustEffects)
		effect->Render();
}

/*************************************/ // Handle

void BrokenPartiableBridge::HandleMovement()
{
	D3DXVECTOR2 position = animation->Position();

	if (BRender() == true)
	{
		for (DustEffect* effect : dustEffects)
			effect->SetBRender(true);

		timeDelay++;
		if (timeDelay >= maxTimeDelay)
		{
			Fall(position);
		}
	}
	else
	{
		AdjustPosition(position);
	}

	animation->Position(position);
}

/*************************************/ // Physics

void BrokenPartiableBridge::Fall(D3DXVECTOR2& _pos)
{
	velocity += gravity * Timer->Elapsed();

	_pos.y += velocity;

	// ÂøÁö Á¶°Ç 
	if (_pos.y <= GroundPosition())
	{
		_pos.y = GroundPosition();
	}
}

/*************************************/ // Adjust Position

void BrokenPartiableBridge::AdjustPosition(D3DXVECTOR2 & _pos)
{
	D3DXVECTOR2 bridgePos = SmallPartiableBridgePos();

	_pos.x = bridgePos.x + offsetX;

}
