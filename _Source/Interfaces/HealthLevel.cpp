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

		����:
			Player�� Health Level�� ǥ���ϴ� Interface�� �����մϴ�.
		����:
			�� 25���� Unit���� Player�� 100% Health Level�� ǥ���մϴ�.
			����, -4% ����, 1���� Unit�� �����մϴ�. 

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

	/**************************************************************/ // ��ġ ����

	FollowPlayer();

	/**************************************************************/ // Health Level Units ���� �� ����

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

	����:
		Player�� �̵��� �Բ� ȭ�鿡�� ������ ��ġ�� ����Ű�ʴϴ�.
	����:
		1. �⺻������ Player�� ��ġ�� �������� ��ġ�� �����մϴ�.
		2. Player�� �߰� ���� or ���� ������ ������ Camera�� ���� �Ǿ�,
			�̶�, Camera ��ġ�� �������� ����ϴ�.

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
	����:
		Player Health Level�� ��ȭ�� ����, HealthLevelUnit�� ���� Ȥ�� ���� ��ŵ�ϴ�.
	����:
		1. Unit �� ���� 4%�� �ǹ��մϴ�.
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
