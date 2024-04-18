#pragma once
#include "Effects/Effect.h"

class CrashEffect : public Effect
{
public:
	CrashEffect(D3DXVECTOR2 position);
	virtual ~CrashEffect();

	virtual void Update(D3DXMATRIX& V, D3DXMATRIX& P) override;

	virtual void Render() override;

	Sprite* GetSprite() { return animation->GetSprite(); }
	Clip* GetClip() { return animation->GetClip(); }

public:
	void HandleStateNumber();

public:
	void DelayDeadState();
	void CheckDeadStateCondition();

public:
	void AdjustPosition(bool _adjustable);

public:

	void SetStateNumber(int i) { stateNumber = i; }
	int StateNumber() { return stateNumber; }

	void SetBDead(bool b) { bDead = b; }
	bool BDead() { return bDead; }
	
	void SetBAdjustable(bool b) { bAdjustable = b; }
	bool BAdjustable() { return bAdjustable; }

public:
	void SetOffsetPositionX(float f) { offsetPositionX = f; }
	float OffsetPositionX() { return offsetPositionX; }

	void SetOffsetPositionY(float f) { offsetPositionY = f; }
	float OffsetPositionY() { return offsetPositionY; }

private:
	Animation* animation;

private:
	int stateNumber;
	int frameCycle;

private:
	bool bDead;
	bool bAdjustable = false;

private:
	const int positionX = 10;

	float offsetPositionX = 0.f;
	float offsetPositionY = 0.f;

};