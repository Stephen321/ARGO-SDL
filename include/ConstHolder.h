#pragma once

#include "EntityType.h"

const int SCREEN_FPS = 60;
const float CAMERA_SYSTEM_UPDATE = 16.f / 1000.f;
const float COLLISION_SYSTEM_UPDATE = 16.f / 1000.f;

const int WORLD_WIDTH = 6400;
const int WORLD_HEIGHT = 5120;

const float DRAG = 0.95f;

const int PIXELS_PER_METER = 100;

const int MAX_FLAG_VELOCITY = 100;
const int MAX_PLAYER_VELOCITY = 10;
const float PLAYER_ACCEL_RATE = 2.0f;

const int MAX_AI_VELOCITY = 6;
const float AI_ACCEL_RATE = 0.5f;

const int MAX_BULLET_VELOCITY = MAX_PLAYER_VELOCITY * 1.5f;
const int BULLET_AMMO = 5;
const float BULLET_FIRE_RATE = 1.0f;


/*
BULLETS NEED TO BE TESTED
WEAPON NEEDS TO BE BINDED WITH A KEY
SPRITE HOLDERS FOR ALL POWER UPS, WEAPONS, BULLETS
SPAWN APPROPRIATE WEAPON WITH APPROPRIATE BULLET BASED ON ID WITH IMPLEMENTATION
POWER UP SPAWNER NEEDS TO BE CREATED
IMPLEMENT NONE WEAPON POWER UPS IN OTHER CODES

PHYSICS NEED FIXING
UI NOT DONE
MENUES ARE SHIT
*/

const uint16 PLAYER_MASK = (uint16)EntityType::AI | (uint16)EntityType::Bullet | (uint16)EntityType::Checkpoint | (uint16)EntityType::Flag | (uint16)EntityType::Obstacle | (uint16)EntityType::PowerUp;
const uint16 AI_MASK = (uint16)EntityType::Player | (uint16)EntityType::AI | (uint16)EntityType::Bullet | (uint16)EntityType::Checkpoint | (uint16)EntityType::Flag | (uint16)EntityType::Obstacle | (uint16)EntityType::PowerUp;
const uint16 BULLET_SENSOR_MASK = (uint16)EntityType::AI | (uint16)EntityType::Player; 
const uint16 BULLET_BODY_MASK = (uint16)EntityType::Obstacle;
const uint16 CHECKPOINT_MASK = (uint16)EntityType::Player | (uint16)EntityType::AI;
const uint16 FLAG_SENSOR_MASK = (uint16)EntityType::Player | (uint16)EntityType::AI;
const uint16 FLAG_BODY_MASK = (uint16)EntityType::Obstacle;
const uint16 POWERUP_MASK = (uint16)EntityType::Player | (uint16)EntityType::AI;
const uint16 OBSTACLE_MASK = (uint16)EntityType::Player | (uint16)EntityType::AI | (uint16)EntityType::Bullet | (uint16)EntityType::Flag;

const float PLAYER_WALL_RESTITUTION = 0.8f;
const float PLAYER_STATIONARY_PLAYER_RESTITUTION = 0.8f;
const float PLAYER_HEAD_ON_PLAYER_RESTITUTION = 0.6f;

const int FLAG_CONNECTION_RADIUS = 400;

const int CHECKPOINT_COLLISION_RADIUS = 300;

const int AI_TO_CHASE_RANGE_OFFSET = 200;
const int AI_FLAG_DETECTION_RADIUS = 250;
const int AI_NODE_COLLISION_RADIUS = 250;


const float STAGGER_MAX_TIMER = 2.0f;
const float INVINCIBLE_MAX_TIMER = 2.0f;
const float INVISIBLE_MAX_TIMER = 2.0f;
const float SPEED_UP_MAX_TIMER = 2.0f;

const int SCREEN_WIDTH = 1600;
const int SCREEN_HEIGHT = 900;


