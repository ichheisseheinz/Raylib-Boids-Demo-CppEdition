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
	if (boid->position.x < -SIZE)
	{
		boid->position.x = SCREEN_WIDTH + SIZE;
	}
	else if (boid->position.x > SCREEN_WIDTH + SIZE)
	{
		boid->position.x = -SIZE;
	}

	if (boid->position.y < -SIZE)
	{
		boid->position.y = SCREEN_HEIGHT + SIZE;
	}
	else if (boid->position.y > SCREEN_HEIGHT + SIZE)
	{
		boid->position.y = -SIZE;
	}
}

Vector2 Alignment(Boid* boid, Boid boids[])
{
	const int radius{ 100 };
	Vector2 steering = Vector2Zero();
	int total{ 0 };

	for (int i = 0; i < MAX_BOIDS; i++)
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
		steering = Vector2Scale(Vector2Normalize(steering), MAX_SPEED);
		steering = Vector2Subtract(steering, boid->velocity);
		steering = Vector2ClampValue(steering, 0, MAX_FORCE);
	}

	return steering;
}

Vector2 Separation(Boid* boid, Boid boids[])
{
	const int radius{ 100 };
	Vector2 steering = Vector2Zero();
	int total{ 0 };

	for (int i = 0; i < MAX_BOIDS; i++)
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
		steering = Vector2Scale(Vector2Normalize(steering), MAX_SPEED);
		steering = Vector2Subtract(steering, boid->velocity);
		steering = Vector2ClampValue(steering, 0, MAX_FORCE);
	}

	return steering;
}

Vector2 Cohesion(Boid* boid, Boid boids[])
{
	const int radius{ 100 };
	Vector2 steering = Vector2Zero();
	int total{ 0 };

	for (int i = 0; i < MAX_BOIDS; i++)
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
		steering = Vector2Scale(Vector2Normalize(steering), MAX_SPEED);
		steering = Vector2Subtract(steering, boid->velocity);
		steering = Vector2ClampValue(steering, 0, MAX_FORCE);
	}

	return steering;
}

void Flock(Boid* boid, Boid boids[])
{
	Vector2 alignment = Alignment(boid, boids);
	Vector2 separation = Separation(boid, boids);
	Vector2 cohesion = Cohesion(boid, boids);

	boid->acceleration = Vector2Add(boid->acceleration, alignment);
	boid->acceleration = Vector2Add(boid->acceleration, separation);
	boid->acceleration = Vector2Add(boid->acceleration, cohesion);
}

void Update(Boid* boid, Boid boids[])
{
	CheckEdges(boid);

	Flock(boid, boids);

	boid->velocity = Vector2Add(boid->velocity, boid->acceleration);
	boid->velocity = Vector2ClampValue(boid->velocity, 0, MAX_SPEED);
	boid->position = Vector2Add(boid->position, boid->velocity);
	boid->acceleration = Vector2Zero();

	boid->rotation = Vector2Angle(Vector2{ 1, 0 }, boid->velocity) * RAD2DEG;
}

void Render(Boid boid)
{
	DrawPolyLines(boid.position, 3, SIZE, boid.rotation, BLACK);
}
