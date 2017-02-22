#pragma once

typedef unsigned short uint16;

enum class EntityType : uint16
{ //draw order is determined by the below order
	Tile = 0x0001,			//1
	Obstacle = 0x0002,		//10
	Checkpoint = 0x0004,	//100

	PowerUp = 0x0008,
	AI = 0x00010,
	Player = 0x0020,
	Bullet = 0x0040,
	Weapon = 0x0080,
	Flag = 0x0100,
	
	Point = 0x0200,
	UI = 0x0800
};