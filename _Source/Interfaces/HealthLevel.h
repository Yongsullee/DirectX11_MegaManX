#pragma once
#include "Interfaces/Interface.h"

class HealthLevel : public Interface
{
public:
	HealthLevel(int initHealthLevel);
	virtual ~HealthLevel();

	virtual void Update(D3DXMATRIX& V, D3DXMATRIX& P);

	virtual void Render();

public:
	void FollowPlayer();

public:
	void IncreaseHealthLevel(int num);
	void DecreaseHealthLevel(int num);

	/*********************************************************/ // Getter & Setter

public:
	void SetPlayerHealthLevel(class Player& player);

public:
	void SetCameraSetToSubBoss1(bool b) { cameraSetToSubBoss1 = b; }
	bool CameraSetToSubBoss1() { return cameraSetToSubBoss1; }

	void SetCameraSetToSubBoss2(bool b) { cameraSetToSubBoss2 = b; }
	bool CameraSetToSubBoss2() { return cameraSetToSubBoss2; }

	void SetCameraSetToBoss(bool b) { cameraSetToBoss = b; }
	bool CameraSetToBoss() { return cameraSetToBoss; }

private:
	Sprite* healthLevelGauge;
	vector<Sprite*> healthLevelUnits;

private:
	int playerHealthLevel;

private:
	bool cameraSetToSubBoss1 = false;
	bool cameraSetToSubBoss2 = false;
	bool cameraSetToBoss = false;

	const float SET_TO_SUBBOSS1 = -3400.f;
	const float SET_TO_SUBBOSS2 = -1490.f;
	const float SET_TO_BOSS = 11170.f;

};