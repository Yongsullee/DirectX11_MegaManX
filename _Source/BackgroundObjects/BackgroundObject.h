#pragma once

class BackgroundObject
{
public:
	BackgroundObject();
	virtual ~BackgroundObject();

public:
	bool BCollide() { return bCollide; }
	void SetBCollide(bool b) { bCollide = b; }

public:
	void GetPlayerPos(class Player& player);

protected:
	D3DXVECTOR2 playerPosition;

private:
	bool bCollide;
};