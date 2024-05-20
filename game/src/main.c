#include "mathf.h"
#include "raylib.h"
#include "raymath.h"
#include "world.h"
#include "integrator.h"
#include "force.h"
#include "render.h"
#include "editor.h"
#include "spring.h"
#include "collision.h"
#include "contact.h"

#include <stdlib.h>
#include <assert.h>


int main(void)
{
	ncBody* selectedBody = NULL;
	ncBody* connectBody = NULL;

	float fixedTimestep = 1.0f / 50;
	float timeAccumulator = 0;

	InitWindow(1920, 1080, "Physics Engine");
	InitEditor();
	SetTargetFPS(60);

	// initialize world
	ncGravity = (Vector2){ 0, -1 };

	// game loop
	while (!WindowShouldClose())
	{
		// update
		float dt = GetFrameTime();
		float fps = (float)GetFPS();

		Vector2 position = GetMousePosition();
		ncScreenZoom += GetMouseWheelMove() * 0.2f;
		ncScreenZoom = Clamp(ncScreenZoom, 0.1, 10);

		UpdateEditor(position);

		selectedBody = GetBodyIntersect(ncBodies, position);
		if (selectedBody)
		{
			Vector2 screen = ConvertWorldToScreen(selectedBody->position);
			DrawCircleLines(screen.x, screen.y, ConvertWorldToPixel(selectedBody->mass * 0.5f) + 5, YELLOW);
		}


		// Create body
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && IsKeyDown(KEY_LEFT_CONTROL)))
		{
			int bodyCount = GetRandomValue(100, 200);
			bodyCount = 1;
			for (int i = 0; i < bodyCount; i++)
			{
				ncBody* body = CreateBody(ConvertScreenToWorld(position), ncEditorData.MassMinSliderValue, ncEditorData.BodyTypeDropdownActive);
				body->damping = 0;//0.5f;
				body->gravityScale = ncEditorData.GravityScaleSliderValue;
				body->color = GREEN;
				body->restitution = 0.3f;
				AddBody(body);
			}
			//CreateRandomFirework(position);
		}

		// connect spring
		if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && selectedBody)
		{
			connectBody = selectedBody;
		}
		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && connectBody)
		{
			DrawLineBodyToPosition(connectBody, position);
		}
		if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) && connectBody)
		{
			if (selectedBody && selectedBody != connectBody)
			{
				ncSpring_t* spring = CreateSpring(selectedBody, connectBody, Vector2Distance(selectedBody->position, connectBody->position), 5);
				AddSpring(spring);
			}
		}

		ApplyGravitation(ncBodies, ncEditorData.GravitationSliderValue);
		ApplySpringForce(ncSprings);

		timeAccumulator += dt;
		ncContact_t* contacts = NULL;
		while (timeAccumulator >= fixedTimestep)
		{
			timeAccumulator -= fixedTimestep;


			// update bodies
			for (ncBody* body = ncBodies; body; body = body->next)
			{
				Step(body, dt);
			}

			//collision
			
			//DestroyAllContacts(contacts);
			CreateContacts(ncBodies, &contacts);
			SeparateContacts(contacts);
			ResolveContacts(contacts);


		}

		//render




















		BeginDrawing();
		ClearBackground(BLACK);



		// Stats
		DrawText(TextFormat("FPS: %.2f (%.2fms)", fps, fps/1000), 10, 10, 20, LIME);
		DrawText(TextFormat("FRAME: %.2f", dt), 10, 30, 20, LIME);

		
		//DrawCircle(position.x, position.y, 10, RED);

		// draw bodies
		for (ncBody* body = ncBodies; body; body = body->next)
		{
			Vector2 screen = ConvertWorldToScreen(body->position);
			DrawCircle(screen.x, screen.y, ConvertWorldToPixel(body->mass * 0.5f), body->color);
		}
		// draw contacts
		for (ncContact_t* contact = contacts; contact; contact = contact->next)
		{
			Vector2 screen = ConvertWorldToScreen(contact->body1->position);
			DrawCircle(screen.x, screen.y, ConvertWorldToPixel(contact->body1->mass * 0.5f), RED);
		}
		// draw springs
		for (ncSpring_t* spring = ncSprings; spring; spring = spring->next)
		{
			Vector2 screen1 = ConvertWorldToScreen(spring->body1->position);
			Vector2 screen2 = ConvertWorldToScreen(spring->body2->position);
			DrawLine(screen1.x, screen1.y, screen2.x, screen2.y, BLUE);
		}

		DrawEditor(position);

		EndDrawing();
	}
	CloseWindow();

	return 0;
}

