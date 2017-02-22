#pragma once

#include "EntityType.h"

const int SCREEN_FPS = 60;
const float CAMERA_SYSTEM_UPDATE = 16.f / 1000.f;
const float COLLISION_SYSTEM_UPDATE = 16.f / 1000.f;

const int WORLD_WIDTH = 6400;
const int WORLD_HEIGHT = 5120;

const float DRAG = 0.95f;

const int PIXELS_PER_METER = 100;

const int MAX_PLAYER_VELOCITY = 10;
const int PLAYER_ACCEL_RATE = 2;


const int MAX_BULLET_VELOCITY = MAX_PLAYER_VELOCITY * 1.5f;
const int BULLET_AMMO = 5;
const float BULLET_FIRE_RATE = 1.0f;

const uint16 PLAYER_MASK = (uint16)EntityType::AI | (uint16)EntityType::Bullet | (uint16)EntityType::Checkpoint | (uint16)EntityType::Flag | (uint16)EntityType::Obstacle | (uint16)EntityType::PowerUp;
const uint16 AI_MASK = (uint16)EntityType::AI | (uint16)EntityType::Bullet | (uint16)EntityType::Checkpoint | (uint16)EntityType::Flag | (uint16)EntityType::Obstacle | (uint16)EntityType::PowerUp;; //not used yet
const uint16 BULLET_MASK = (uint16)EntityType::AI | (uint16)EntityType::Obstacle | (uint16)EntityType::Player; 
const uint16 CHECKPOINT_MASK = (uint16)EntityType::Player;
const uint16 FLAG_MASK = (uint16)EntityType::Player;
const uint16 POWERUP_MASK = 0; //not used yet
const uint16 OBSTACLE_MASK = (uint16)EntityType::Player | (uint16)EntityType::AI | (uint16)EntityType::Bullet;

const float PLAYER_WALL_RESTITUTION = 0.5f;

const int FLAG_CONNECTION_RADIUS = 500;

const int AI_DETECTION_RADIUS = 180;
