#ifndef BOID_H
#define BOID_H

#include "raylib.h"
#include "raymath.h"

#define SCREEN_WIDTH 1080
#define SCREEN_HEIGHT 720

#define MAX_BOIDS 500

#define MAX_SPEED 10
#define SIZE 10
#define MAX_FORCE 0.2

typedef struct Boid
{
	Vector2 position{};
	float rotation{};
	Vector2 velocity{};
	Vector2 acceleration{ Vector2Zero() };
} Boid;

Boid CreateBoid(Vector2 position, int rotation);
void CheckEdges(Boid* boid);
void Update(Boid* boid, Boid boids[]);
void Render(Boid boid);

#endif