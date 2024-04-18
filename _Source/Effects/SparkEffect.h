#pragma once
#include "Effect.h"

class SparkEffect : public Effect
{
public:
	SparkEffect(D3DXVECTOR2 position);
	virtual ~SparkEffect();

	virtual void Update(D3DXMATRIX& V, D3DXMATRIX& P) override;

	virtual void Render() override;

	void SetStateNumber(int i) { stateNumber = i; }
	int StateNumber() { return stateNumber; }

	void SetAngle(float f) { angle = f; }
	float Angle() { return angle; }

private:
	Animation* animation;

private:
	int stateNumber;

private:
	const float PositionX = 40.f;
	const float PositionY = -30.f;

private:
	float angle;

};