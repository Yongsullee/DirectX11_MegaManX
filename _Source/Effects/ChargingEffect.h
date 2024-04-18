#pragma once
#include "Effects/Effect.h"
class ChargingEffect : public Effect
{
public:
	ChargingEffect(D3DXVECTOR2 position);
	virtual ~ChargingEffect();

	virtual void Update(D3DXMATRIX& V, D3DXMATRIX& P) override;

	virtual void Render() override;

	void SetStateNumber(int i) { stateNumber = i; }
	int StateNumber() { return stateNumber; }

private:
	Animation* animation;

private:
	int stateNumber;
};