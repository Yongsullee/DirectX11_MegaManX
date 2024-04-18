#include "stdafx.h"
#include "Objects/Player.h"
#include "SmallPartiableBridge.h"
#include "BrokenPartiableBridge.h"

SmallPartiableBridge::SmallPartiableBridge(int size, D3DXVECTOR2& position)
	:Bridge()
	, sizeNumber(size)
{
	SetBCollide(false);
	SetBPartiable(true);

	wstring textureFile = Textures + L"/MegaManX/Background/Stage1.png";
	wstring shaderFile = Shaders + L"009_Sprite.fx";

	animation = new Animation;

	Clip* clip;

	// BrokenPartiableBridge 생성

	BrokenPartiableBridge* bridge;

	switch (sizeNumber)
	{
	case 0:
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(textureFile, shaderFile, 3964, 238, 3964 + 48, 238 + 64), 0.3f);
		animation->AddClip(clip);

		bridge = new BrokenPartiableBridge(position);
		bridge->SetSmallPartiableBridgeSize(sizeNumber);
		brokenPartiableBridges.push_back(bridge);
		break;
	case 1:
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(textureFile, Shaders + L"009_Sprite.fx", 4412, 238, 4412 + 49, 238 + 64), 0.3f);
		animation->AddClip(clip);

		bridge = new BrokenPartiableBridge(position);
		bridge->SetSmallPartiableBridgeSize(sizeNumber);
		brokenPartiableBridges.push_back(bridge);
		break;
	case 2: 
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(textureFile, Shaders + L"009_Sprite.fx", 4508, 238, 4508 + 81, 238 + 64), 0.3f);
		animation->AddClip(clip);

		for (int i = 0; i < 2; i++)
		{
			bridge = new BrokenPartiableBridge(position);
			bridge->SetSmallPartiableBridgeSize(sizeNumber);
			brokenPartiableBridges.push_back(bridge);
		}
		break;
	case 3:
		clip = new Clip(PlayMode::Loop);
		clip->AddFrame(new Sprite(textureFile, Shaders + L"009_Sprite.fx", 4603, 238, 4603 + 114, 238 + 64), 0.3f);
		animation->AddClip(clip);

		for (int i = 0; i < 3; i++)
		{
			bridge = new BrokenPartiableBridge(position);
			bridge->SetSmallPartiableBridgeSize(sizeNumber);
			brokenPartiableBridges.push_back(bridge);
		}
		break;
	default: 
		break;
	}

	animation->Scale(3.0f, 3.0f);
	animation->Position(position);
	animation->Play(StateNumber());
	animation->DrawBound(false);

}

SmallPartiableBridge::~SmallPartiableBridge()
{
	for (BrokenPartiableBridge* bridge : brokenPartiableBridges)
		SAFE_DELETE(bridge);
}

void SmallPartiableBridge::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	D3DXVECTOR2 position = animation->Position();

	__super::Update(V, P);

	animation->Play(StateNumber());
	animation->Position(position);

	/*************************************/ // Broken Partiable Bridge
	
	HandleBrokenPartiableBridgePosition();
	HandleBrokenParitableBridgeRender();

	for (BrokenPartiableBridge* bridge : brokenPartiableBridges)
	{
		bridge->SetSmallPartiableBridgePos(position);
		bridge->Update(V, P);
	}

}

void SmallPartiableBridge::Render()
{

	if(BRender() == true)
		__super::Render();

	/*************************************/ // Broken Partiable Bridge

	for (BrokenPartiableBridge* bridge : brokenPartiableBridges)
		bridge->Render();

}

/*************************************/ // Broken Partiable Bridge

void SmallPartiableBridge::HandleBrokenPartiableBridgePosition()
{
	// Offset 설정
	switch (sizeNumber)
	{
	case 0:
		brokenPartiableBridges.at(0)->SetOffsetX(offsetX0);
		break;
	case 1:
		brokenPartiableBridges.at(0)->SetOffsetX(offsetX1);
		break;
	case 2:
		brokenPartiableBridges.at(0)->SetOffsetX(offsetX2_1);
		brokenPartiableBridges.at(1)->SetOffsetX(offsetX2_2);
		break;
	case 3:
		brokenPartiableBridges.at(0)->SetOffsetX(offsetX3_1);
		brokenPartiableBridges.at(1)->SetOffsetX(offsetX3_2);
		brokenPartiableBridges.at(2)->SetOffsetX(offsetX3_3);
		break;
	default:
		break;
	}
	
}

void SmallPartiableBridge::HandleBrokenParitableBridgeRender()
{
	if (BBroken() == true)
	{
		frameCycle++;
		if (frameCycle >= 20)
		{
			SetBRender(false);

			for (BrokenPartiableBridge* bridge : brokenPartiableBridges)
				bridge->SetBRender(true);
		}
	}
}
