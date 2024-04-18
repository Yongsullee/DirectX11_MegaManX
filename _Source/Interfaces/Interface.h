#pragma once

class Interface
{
public:
	Interface();
	virtual ~Interface();

	virtual void Update(D3DXMATRIX& V, D3DXMATRIX& P);

	virtual void Render();

public:
	void GetPlayerPos(class Player& player);

protected:
	D3DXVECTOR2 playerPosition;
};