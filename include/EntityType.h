#pragma once

typedef unsigned short uint16;

enum class EntityType : uint16
{ //draw order is determined by the below order
	Flag = 0x0001,  //1
	Checkpoint = 0x0002, // 10
	Wall = 0x0004, // 100
	Tile = 0x0008, //1000
	Obstacle = 0x0010,
	Bullet = 0x0020,
	PowerUp = 0x0040,
	AI = 0x0080,
	Player = 0x0100,
	Point = 0x0200,
	Weapon = 0x0400,
};
