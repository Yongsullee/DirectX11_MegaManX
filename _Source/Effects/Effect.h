#pragma once

class Effect
{
public:
	Effect();
	virtual ~Effect();

	virtual void Update(D3DXMATRIX& V, D3DXMATRIX& P);
	
	virtual void Render();

public:
	void GetPlayerPos(class Player& _player);
	void GetEnemyPos(D3DXVECTOR2& _position);
	void GetWeaponPos(D3DXVECTOR2& _position);

public:
	void SetBRender(bool b) { bRender = b; }
	bool BRender() { return bRender; }

protected:
	D3DXVECTOR2 playerPos;
	D3DXVECTOR2 enemyPos;
	D3DXVECTOR2 weaponPos;

	bool bRender = false;
};