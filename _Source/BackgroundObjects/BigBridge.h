#pragma once
#include "Bridge.h"

class BigBridge : public Bridge
{
public:
	BigBridge();
	virtual ~BigBridge();

	virtual void Update(D3DXMATRIX& V, D3DXMATRIX& P);

	virtual void Render();

};