#pragma once
#include "Effects/Effect.h"

class FlyingDustEffect : public Effect
{
public:
	FlyingDustEffect(D3DXVECTOR2 position);
	virtual ~FlyingDustEffect();

	virtual void Update(D3DXMATRIX& V, D3DXMATRIX& P) override;

	virtual void Render() override;

	Sprite* GetSprite() { return animation->GetSprite(); }
	Clip* GetClip() { return animation->GetClip(); }

public:
	void HandleStateNumber();

	void DelayDeadState();
	void CheckDeadStateCondition();

public:

	void AdjustPosition(bool _adjustable);

	/*************************************/ // Getter & Setter

public:
	void GetWeaponPos(D3DXVECTOR2 _pos) { weaponPos = _pos; }

	void SetStateNumber(int i) { stateNumber = i; }
	int StateNumber() { return stateNumber; }

	void SetBDead(bool b) { bDead = b; }
	bool BDead() { return bDead; }

	void SetBAdjustable(bool b) { bAdjustable = b; }
	bool BAdjustable() { return bAdjustable; }

	void SetOffsetPositionX(float f) { offsetPositionX = f; }
	float OffsetPositionX() { return offsetPositionX; }

	void SetOffsetPositionY(float f) { offsetPositionY = f; }
	float OffsetPositionY() { return offsetPositionY; }

public:

private:
	Animation* animation;

private:
	int stateNumber;
	int frameCycle;

private:
	bool bDead;

private:
	bool bAdjustable = false;

private:
	D3DXVECTOR2 weaponPos;

	const int positionX = 10;

	float offsetPositionX = 0.f;
	float offsetPositionY = 0.f;

};