#pragma once
#include "Bridge.h"

class BrokenPartiableBridge : public Bridge
{
public:
	BrokenPartiableBridge(D3DXVECTOR2 _pos);
	virtual ~BrokenPartiableBridge();

	virtual void Update(D3DXMATRIX& V, D3DXMATRIX& P) override;

	virtual void Render() override;

	/*************************************/ // Handle 

public:
	void HandleMovement();

	/*************************************/ // Physics

public:
	virtual void Fall(D3DXVECTOR2& _pos);

	/*************************************/ // AdjustPosition

	void AdjustPosition(D3DXVECTOR2& _pos);

	/*************************************/ // Getter & Setter

	void SetSmallPartiableBridgeSize(int i) { smallPartiableBridgeSize = i; }
	int SmallPartiableBridgeSize() { return smallPartiableBridgeSize; }

	void SetSmallPartiableBridgePos(D3DXVECTOR2& _pos) { smallPartiableBridgePos = _pos; }
	D3DXVECTOR2 SmallPartiableBridgePos() { return smallPartiableBridgePos; }

	void SetOffsetX(float f) { offsetX = f; }
	float OffsetX() { return offsetX; }

	void SetBRender(bool b) { bRender = b; }
	bool BRender() { return bRender; }

	void SetGroundPosition(float f) { groundPosition = f; }
	float GroundPosition() { return groundPosition; }


private:
	D3DXVECTOR2 smallPartiableBridgePos = D3DXVECTOR2(0.f, 0.f);
	int smallPartiableBridgeSize = -1;

	float offsetX = 0.f;

private:
	bool bRender = false;

private:
	float groundPosition = -800.f;

private:
	float velocity = 0.f;
	float gravity = -30.f;

private:
	int timeDelay = 0;
	const int maxTimeDelay = 15;

	/***************************************/ //Effect

private:
	vector<class DustEffect*> dustEffects;

	const float effectOffsetX_0 = -50.f;	// Size Number가 0일 경우
	const float effectOffsetX_1 = 50.f;		// Size Number가 0이 아닐 경우
};