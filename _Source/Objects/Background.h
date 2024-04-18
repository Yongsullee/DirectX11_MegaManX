#pragma once

class Background
{
public:
	Background(wstring shaderFile);
	~Background();

	void Update(D3DXMATRIX& V, D3DXMATRIX& P);
	void Render();

	void BackgroundMove(class Player& inPlayerInfo);

private:
	Sprite* bg1;

	class BigBridge* bgBridge;

private:
	class Player* playerInfo;

	float maxPlayerPos = -10600.f;

};