#pragma once

typedef unsigned short uint16;

enum class EntityType : uint16
{ //draw order is determined by the below order
	Tile = 0x0001,			//0000000000000001
	Obstacle = 0x0002,		//0000000000000010
	Checkpoint = 0x0004,	//0000000000000100
	PowerUp = 0x0008,		//0000000000001000
	Weapon = 0x00010,
	AI = 0x00020,
	Player = 0x0040,
	RemotePlayer = 0x0080,
	Bullet = 0x0100,
	Flag = 0x0200,
	Point = 0x0400,
	UI = 0x0800,
}; //16 max
