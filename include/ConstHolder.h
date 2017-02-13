#pragma once

const int SCREEN_FPS = 60;
const float CAMERA_SYSTEM_UPDATE = 16.f / 1000.f;
const float COLLISION_SYSTEM_UPDATE = 16.f / 1000.f;

const int WORLD_WIDTH = 3000;
const int WORLD_HEIGHT = 3200;

const float DRAG = 0.95f;

const int PIXELS_PER_METER = 100;

const int MAX_PLAYER_VELOCITY = 10;
const int PLAYER_ACCEL_RATE = 2;

const int MAX_BULLET_VELOCITY = MAX_PLAYER_VELOCITY * 1.5f;
const int BULLET_AMMO = 5;
const float BULLET_FIRE_RATE = 1.0f;
