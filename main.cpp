#include "raylib.h"

#include "boid.h"

Boid _boids[MAX_BOIDS];

void CreateBoids()
{
	for (int i = 0; i < MAX_BOIDS; i++)
	{
		_boids[i] = CreateBoid(
			Vector2{ (float)GetRandomValue(0, SCREEN_WIDTH), (float)GetRandomValue(0, SCREEN_HEIGHT) },
			GetRandomValue(0, 359));
	}
}

int main()
{
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Boids demo");
	SetTargetFPS(60);

	CreateBoids();

	while (!WindowShouldClose())
	{
		for (int i = 0; i < MAX_BOIDS; i++)
		{
			Update(_boids + i, _boids);
		}

		BeginDrawing();

		ClearBackground(RAYWHITE);

		for (int i = 0; i < MAX_BOIDS; i++)
		{
			Render(_boids[i]);
		}

		DrawFPS(10, 10);

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
