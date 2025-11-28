#include "raylib.h"

#include "boid.h"

Boid _boids[maxBoids];

const int screenWidth = 1080;
const int screenHeight = 720;

void CreateBoids()
{
	for (int i = 0; i < maxBoids; i++)
	{
		_boids[i] = CreateBoid(
			Vector2{ (float)GetRandomValue(0, screenWidth), (float)GetRandomValue(0, screenHeight) },
			GetRandomValue(0, 359));
	}
}

int main()
{

	InitWindow(screenWidth, screenHeight, "Boids demo");
	SetTargetFPS(60);

	CreateBoids();

	while (!WindowShouldClose())
	{
		for (int i = 0; i < maxBoids; i++)
		{
			Update(_boids + i, _boids);
		}

		BeginDrawing();

		ClearBackground(RAYWHITE);

		for (int i = 0; i < maxBoids; i++)
		{
			Render(_boids[i]);
		}

		DrawFPS(10, 10);

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
