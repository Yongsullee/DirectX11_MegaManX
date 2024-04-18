#pragma once
#include "Bridge.h"

class SmallPartiableBridge : public Bridge
{
public:
	SmallPartiableBridge(int size, D3DXVECTOR2& position);
	virtual ~SmallPartiableBridge();

	virtual void Update(D3DXMATRIX& V, D3DXMATRIX& P) override;

	virtual void Render() override;

	/*************************************/ // Handle 

	void HandleBrokenPartiableBridgePosition();
	void HandleBrokenParitableBridgeRender();

	/*************************************/ // Getter & Setter

	void SetBRender(bool b) { bRender = b; }
	bool BRender() { return bRender; }

private:
	int sizeNumber;
	bool bRender = true;

	int frameCycle = 0;

public:
	vector<class BrokenPartiableBridge*> brokenPartiableBridges;

private:
	const float offsetX0 = 10.f;
	
	const float offsetX1 = -10.f;
	
	const float offsetX2_1 = -50.f;
	const float offsetX2_2 = +50.f;

	const float offsetX3_1 = -100.f;
	const float offsetX3_2 = 0.f;
	const float offsetX3_3 = 100.f;

};