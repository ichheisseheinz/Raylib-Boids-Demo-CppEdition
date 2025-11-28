#pragma once

#include "raylib.h"
#include "raymath.h"

constexpr int maxBoids = 500;

constexpr int maxSpeed = 10;
constexpr int size = 10;
constexpr float maxForce = 0.2f;

struct Boid
{
	Vector2 position{};
	float rotation{};
	Vector2 velocity{};
	Vector2 acceleration{ Vector2Zero() };
};

Boid CreateBoid(Vector2 position, int rotation);
void CheckEdges(Boid* boid);
void Update(Boid* boid, Boid boids[]);
void Render(Boid boid);