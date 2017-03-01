#pragma once

#include "EntityType.h"

const int SCREEN_FPS = 60;
const float CAMERA_SYSTEM_UPDATE = 16.f / 1000.f;
const float PHYSICS_SYSTEM_UPDATE = 0.f;// 16.f / 1000.f;
const float COLLISION_SYSTEM_UPDATE = 16.f / 1000.f;
const float REMOTE_PACKET_RATE = (1000.f / 10.f) / 1000.f; //10 packets a second

const int		WORLD_WIDTH = 3200;
const int		WORLD_HEIGHT = 3200;

const float		DRAG = 0.95f;

const int		PIXELS_PER_METER = 100;

const int		MAX_FLAG_VELOCITY = 100;
const int		MAX_PLAYER_VELOCITY = 10;
const float		PLAYER_ACCEL_RATE = 2.0f;

const float CONNECT_RETRY = 1.5f;

const int		MAX_BULLET_VELOCITY = MAX_PLAYER_VELOCITY * 1.5f;
const int		AMMO[] { 8, 5, 15 };
const float		FIRE_RATE[]{ 1.0f, 2.5f, 0.1f };

const float		PLAYER_WALL_RESTITUTION = 0.8f;
const float		PLAYER_STATIONARY_PLAYER_RESTITUTION = 0.8f;
const float		PLAYER_HEAD_ON_PLAYER_RESTITUTION = 0.6f;


const uint16 CHARACTER_MASK = (uint16)EntityType::Remote | (uint16)EntityType::AI | (uint16)EntityType::Player;
const uint16 PLAYER_MASK =  CHARACTER_MASK | (uint16)EntityType::Bullet | (uint16)EntityType::Checkpoint | (uint16)EntityType::Flag | (uint16)EntityType::Obstacle | (uint16)EntityType::PowerUp;
const uint16 AI_MASK =		CHARACTER_MASK | (uint16)EntityType::Bullet | (uint16)EntityType::Checkpoint | (uint16)EntityType::Flag | (uint16)EntityType::Obstacle | (uint16)EntityType::PowerUp;
const uint16 REMOTE_MASK =  CHARACTER_MASK | (uint16)EntityType::Bullet | (uint16)EntityType::Checkpoint | (uint16)EntityType::Flag | (uint16)EntityType::PowerUp;
const uint16 BULLET_SENSOR_MASK = CHARACTER_MASK;
const uint16 BULLET_BODY_MASK = (uint16)EntityType::Obstacle;
const uint16 CHECKPOINT_MASK = CHARACTER_MASK;
const uint16 FLAG_SENSOR_MASK = CHARACTER_MASK;
const uint16 FLAG_BODY_MASK = (uint16)EntityType::Obstacle;
const uint16 POWERUP_MASK = CHARACTER_MASK;
const uint16 OBSTACLE_MASK = (uint16)EntityType::Player | (uint16)EntityType::AI | (uint16)EntityType::Bullet | (uint16)EntityType::Flag;

const int		FLAG_CONNECTION_RADIUS = 200;
const int		AI_DETECTION_RADIUS = 100;

const float		STAGGER_MAX_TIMER = 2.0f;
const float		INVINCIBLE_MAX_TIMER = 2.0f;
const float		INVISIBLE_MAX_TIMER = 2.0f;
const float		SPEED_UP_MAX_TIMER = 2.0f;
const float		POWER_UP_SPAWN_RATE = 5.0f;

const int		SCREEN_WIDTH = 1600;
const int		SCREEN_HEIGHT = 900;

