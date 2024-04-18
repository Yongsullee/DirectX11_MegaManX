#pragma once
#include "Bridge.h"

class PartiableBridge : public Bridge
{
public:
	PartiableBridge(int num);
	virtual ~PartiableBridge();

	virtual void Update(D3DXMATRIX& V, D3DXMATRIX& P) override;

	virtual void Render() override;

	/***************************************/ //Handle

	void HandleStateNumber();
	void HandleMovement();

	/***************************************/ //State

	void CheckStateNumberChange0to1Condition();

	/***************************************/ //Getter & Setter

	void SetBCollideWithSubBoss(bool b) { bCollideWithSubBoss = b; }
	bool BCollideWithSubBoss() { return bCollideWithSubBoss; }

	void SetGroundPosition(float f) { groundPosition = f; }
	float GroundPosition() { return groundPosition; }

	//구현 필요
public:
	void CheckSubBossCollision();

public:
	void Fall(D3DXVECTOR2& _pos);

private:
	int positionNumber;

private:
	bool bCollideWithSubBoss = false;

private:
	float groundPosition = -800.f;

private:
	float velocity = 0.f;
	float gravity = -50.f;
};