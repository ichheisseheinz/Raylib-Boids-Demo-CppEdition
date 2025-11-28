#include "raylib.h"
#include "raymath.h"

#include "boid.h"

Boid CreateBoid(Vector2 position, int rotation)
{
	return Boid{
		position,
		(float)rotation,
		Vector2Rotate(Vector2{ 1, 0 }, rotation * DEG2RAD)
	};
}

void CheckEdges(Boid* boid)
{
	Vector2 screenDim = Vector2{ (float)GetScreenWidth(), (float)GetScreenHeight() };

	if (boid->position.x < -size)
	{
		boid->position.x = screenDim.x + size;
	}
	else if (boid->position.x > screenDim.x + size)
	{
		boid->position.x = -size;
	}

	if (boid->position.y < -size)
	{
		boid->position.y = screenDim.y + size;
	}
	else if (boid->position.y > screenDim.y + size)
	{
		boid->position.y = -size;
	}
}

Vector2 Alignment(Boid* boid, Boid boids[])
{
	const int radius{ 100 };
	Vector2 steering = Vector2Zero();
	int total{ 0 };

	for (int i = 0; i < maxBoids; i++)
	{
		if (Vector2Distance(boid->position, boids[i].position) < radius && !(boid == &boids[i]))
		{
			steering = Vector2Add(steering, boids[i].velocity);
			total++;
		}
	}

	if (total > 0)
	{
		steering = Vector2Scale(steering, 1.0f / total);
		steering = Vector2Scale(Vector2Normalize(steering), maxSpeed);
		steering = Vector2Subtract(steering, boid->velocity);
		steering = Vector2ClampValue(steering, 0, maxForce);
	}

	return steering;
}

Vector2 Separation(Boid* boid, Boid boids[])
{
	const int radius{ 100 };
	Vector2 steering = Vector2Zero();
	int total{ 0 };

	for (int i = 0; i < maxBoids; i++)
	{
		float dist = Vector2Distance(boid->position, boids[i].position);
		if (dist < radius && !(boid == &boids[i]))
		{
			Vector2 diff = Vector2Subtract(boid->position, boids[i].position);
			diff = Vector2Scale(diff, 1.0f / (dist * dist));
			steering = Vector2Add(steering, diff);
			total++;
		}
	}

	if (total > 0)
	{
		steering = Vector2Scale(steering, 1.0f / total);
		steering = Vector2Scale(Vector2Normalize(steering), maxSpeed);
		steering = Vector2Subtract(steering, boid->velocity);
		steering = Vector2ClampValue(steering, 0, maxForce);
	}

	return steering;
}

Vector2 Cohesion(Boid* boid, Boid boids[])
{
	const int radius{ 100 };
	Vector2 steering = Vector2Zero();
	int total{ 0 };

	for (int i = 0; i < maxBoids; i++)
	{
		if (Vector2Distance(boid->position, boids[i].position) <= radius && !(boid == &boids[i]))
		{
			steering = Vector2Add(steering, boids[i].position);
			total++;
		}
	}

	if (total > 0)
	{
		steering = Vector2Scale(steering, 1.0f / total);
		steering = Vector2Subtract(steering, boid->position);
		steering = Vector2Scale(Vector2Normalize(steering), maxSpeed);
		steering = Vector2Subtract(steering, boid->velocity);
		steering = Vector2ClampValue(steering, 0, maxForce);
	}

	return steering;
}

void Flock(Boid* boid, Boid boids[])
{
	boid->acceleration = Vector2Add(boid->acceleration, Alignment(boid, boids));
	boid->acceleration = Vector2Add(boid->acceleration, Separation(boid, boids));
	boid->acceleration = Vector2Add(boid->acceleration, Cohesion(boid, boids));
}

void Update(Boid* boid, Boid boids[])
{
	CheckEdges(boid);

	Flock(boid, boids);

	boid->velocity = Vector2Add(boid->velocity, boid->acceleration);
	boid->velocity = Vector2ClampValue(boid->velocity, 0, maxSpeed);
	boid->position = Vector2Add(boid->position, boid->velocity);
	boid->acceleration = Vector2Zero();

	boid->rotation = Vector2Angle(Vector2{ 1, 0 }, boid->velocity) * RAD2DEG;
}

void Render(Boid boid)
{
	DrawPolyLines(boid.position, 3, size, boid.rotation, BLACK);
}
