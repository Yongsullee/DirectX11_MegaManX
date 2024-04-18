#include "stdafx.h"
#include "Interfaces/Interface.h"
#include "Objects/Player.h"

Interface::Interface()
	:playerPosition(0.f, 0.f)
{
}

Interface::~Interface()
{
}

void Interface::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
}

void Interface::Render()
{
}

void Interface::GetPlayerPos(Player & player)
{
	playerPosition = player.GetSprite()->Position();
}
