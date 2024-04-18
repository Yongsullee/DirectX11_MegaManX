#include "stdafx.h"
#include "BackgroundObject.h"
#include "Objects/Player.h"

BackgroundObject::BackgroundObject()
	:playerPosition(0.f, 0.f)
	,bCollide(false)
{}

BackgroundObject::~BackgroundObject()
{}

void BackgroundObject::GetPlayerPos(Player & player)
{
	playerPosition = player.GetSprite()->Position();
}
