#include "stdafx.h"
#include "Effects/MakingHoleEffect.h"

MakingHoleEffect::MakingHoleEffect(D3DXVECTOR2 _position, int _direction)
	:Effect()
	, direction(_direction)
	, playerPos(0.f, 0.f)
{
	wstring spriteFile = Textures + L"MegaManX/Effect/MakingHoleEffect.png";
	wstring shaderFile = Shaders + L"009_Sprite.fx";

	switch (_direction)
	{
	case 1:	// 哭率
		sprite = new Sprite(spriteFile, shaderFile, 0, 0, 16, 16);
		break;
	case 2: // 啊款单
		sprite = new Sprite(spriteFile, shaderFile, 17, 0, 33, 16);
		break;
	case 3: // 坷弗率
		sprite = new Sprite(spriteFile, shaderFile, 34, 0, 50, 16);
		break;
	default:
		break;
	}

	sprite->Scale(2.5f, 2.5f);
	sprite->Position(_position);
	sprite->DrawBound(false);
}

MakingHoleEffect::~MakingHoleEffect()
{
	SAFE_DELETE(sprite);
}

void MakingHoleEffect::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	D3DXVECTOR2 position = sprite->Position();
	__super::Update(V, P);

	AdjustPosition(weaponPos);
	
	sprite->Update(V, P);
}

void MakingHoleEffect::Render()
{
	if (BRender() == true)
	{
		__super::Render();
		sprite->Render();
	}
}

void MakingHoleEffect::AdjustPosition(D3DXVECTOR2 _weaponPos)
{
	switch (direction)
	{
	case 1:
		sprite->Position(D3DXVECTOR2(weaponPos.x - 30.f, weaponPos.y));
		break;
	case 2:
		sprite->Position(D3DXVECTOR2(weaponPos.x, weaponPos.y));
		break;
	case 3:
		sprite->Position(D3DXVECTOR2(weaponPos.x + 30.f, weaponPos.y));
		break;
	default:
		break;
	}

}
