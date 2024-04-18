#include "stdafx.h"
#include "Interfaces/HealthLevel.h"
#include "Objects/Player.h"

HealthLevel::HealthLevel(int initHealthLevel)
	:Interface()
	, playerHealthLevel(initHealthLevel)
{
	wstring shaderFile = Shaders + L"009_Sprite.fx";
	wstring spriteFile = L""; 

	D3DXVECTOR2 position = playerPosition;

	spriteFile = Textures + L"MegaManX/Interface/HealthLevelGauge.png";
	healthLevelGauge = new Sprite(spriteFile, shaderFile);
	healthLevelGauge->Scale(3.0f, 3.0f);
	healthLevelGauge->Position(playerPosition.x - 0.9f * 0.5f * Width, 100.f);
	
	/*

		목적:
			Player의 Health Level을 표현하는 Interface를 생성합니다.
		설명:
			총 25개의 Unit으로 Player의 100% Health Level을 표현합니다.
			따라서, -4% 마다, 1개의 Unit을 차감합니다. 

	*/

	for (int i = 0; i < 25; i++)
	{
		spriteFile = Textures + L"MegaManX/Interface/HealthLevelUnit.png";
		Sprite* sprite = new Sprite(spriteFile, shaderFile, 133, 78, 133+6, 78+1);
		sprite->Scale(3.5f, 3.5f);
		sprite->Position(playerPosition.x - 0.9f * 0.5f * Width, 72.f + 4.0f * i);
		healthLevelUnits.push_back(sprite);
	}
}

HealthLevel::~HealthLevel()
{
	SAFE_DELETE(healthLevelGauge);
	for (Sprite* unit : healthLevelUnits)
		SAFE_DELETE(unit);
}

void HealthLevel::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	__super::Update(V, P);

	/**************************************************************/ // 위치 조정

	FollowPlayer();

	/**************************************************************/ // Health Level Units 증가 및 감소

	if (playerHealthLevel > 4 * healthLevelUnits.size())
	{
		if (abs(playerHealthLevel - (4 * (int)healthLevelUnits.size()))/4 > 0)
		{
			IncreaseHealthLevel(abs(playerHealthLevel - (4 * (int)healthLevelUnits.size())) / 4);
		}
	}

	if (playerHealthLevel < 4 * healthLevelUnits.size())
	{
		if (abs(playerHealthLevel - (4 * (int)healthLevelUnits.size()))/4 > 0)
		{
			DecreaseHealthLevel(abs(playerHealthLevel - (4 * (int)healthLevelUnits.size())) / 4);
		}
	}
	
	/**************************************************************/ // Update

	healthLevelGauge->Update(V, P);
	for (Sprite* unit : healthLevelUnits)
		unit->Update(V, P);
}

void HealthLevel::Render()
{
	__super::Render();

	healthLevelGauge->Render();
	for (Sprite* unit : healthLevelUnits)
		unit->Render();
}

/*

	목적:
		Player의 이동과 함께 화면에서 일정한 위치를 유지키십니다.
	설명:
		1. 기본적으로 Player의 위치를 기준으로 위치를 설정합니다.
		2. Player가 중간 보스 or 최종 보스를 만나면 Camera가 고정 되어,
			이때, Camera 위치를 기준으로 삼습니다.

*/

void HealthLevel::FollowPlayer()
{
	D3DXVECTOR2 position = playerPosition;

	if (CameraSetToSubBoss1() == true)
	{
		healthLevelGauge->Position(SET_TO_SUBBOSS1, 100.f);

		for (int i = 0; i < healthLevelUnits.size(); i++)
		{
			healthLevelUnits.at(i)->Position(SET_TO_SUBBOSS1, 72.f + 4.0f * i);
		}
	}
	else if (CameraSetToSubBoss2() == true)
	{
		healthLevelGauge->Position(SET_TO_SUBBOSS2, 100.f);

		for (int i = 0; i < healthLevelUnits.size(); i++)
		{
			healthLevelUnits.at(i)->Position(SET_TO_SUBBOSS2, 72.f + 4.0f * i);
		}
	}
	else if (CameraSetToBoss() == true)
	{
		healthLevelGauge->Position(SET_TO_BOSS, 100.f);

		for (int i = 0; i < healthLevelUnits.size(); i++)
		{
			healthLevelUnits.at(i)->Position(SET_TO_BOSS, 72.f + 4.0f * i);
		}
	}
	else
	{
		healthLevelGauge->Position(playerPosition.x - 0.8f * 0.5f * Width, 100.f);
		for (int i = 0; i < healthLevelUnits.size(); i++)
		{
			healthLevelUnits.at(i)->Position(playerPosition.x - 0.8f * 0.5f * Width, 72.f + 4.0f * i);
		}
	}
}

void HealthLevel::SetPlayerHealthLevel(Player & player)
{
	HealthLevel::playerHealthLevel = player.PlayerHealthLevel();
}

/*
	목적:
		Player Health Level의 변화에 따라, HealthLevelUnit을 증가 혹은 감소 시킵니다.
	설명:
		1. Unit 한 개당 4%를 의미합니다.
		2. 
*/
void HealthLevel::IncreaseHealthLevel(int num)
{
	wstring shaderFile = Shaders + L"009_Sprite.fx";
	wstring spriteFile = Textures + L"MegaManX/Interface/HealthLevelUnit.png";
	int index = healthLevelUnits.size();

	for (int i = 0; i < num; i++)
	{
		Sprite* sprite = new Sprite(spriteFile, shaderFile, 133, 78, 133 + 6, 78 + 1);
		sprite->Scale(3.5f, 3.5f);
		sprite->Position(playerPosition.x - 0.9f * 0.5f * Width, 72.f + 4.0f * (index + i));
		healthLevelUnits.push_back(sprite);
	}
}

void HealthLevel::DecreaseHealthLevel(int num)
{
	for (int i = 0; i < num; i++)
	{
		healthLevelUnits.erase(healthLevelUnits.end()-1);
	}
}
