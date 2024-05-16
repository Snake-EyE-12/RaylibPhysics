#pragma once
#include "raylib.h"
#include "raymath.h"

typedef enum
{
	BT_STATIC,
	BT_KINEMATIC,
	BT_DYNAMIC
} ncBodyType;

typedef enum
{
	FM_FORCE,
	FM_IMPLUSE,
	FM_VELOCITY
} ncForceMode;

typedef struct ncBody
{
	ncBodyType type;

	// acceleration -> velocity -> position
	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;
	Vector2 force;

	float mass;
	float inverseMass; // 1 / mass (static = 0)
	float gravityScale;
	float damping;

	float restitution;

	struct ncBody* next;
	struct ncBody* prev;

	Color color;
} ncBody;

inline void ApplyForce(ncBody* body, Vector2 force, ncForceMode forceMode)
{
	if (body->type != BT_DYNAMIC) return;

	switch (forceMode)
	{
	case FM_FORCE:
		body->force = Vector2Add(body->force, force);
		break;
	case FM_IMPLUSE:
		// applies a sudden change in momentum
		body->velocity = Vector2Add(Vector2Scale(force, body->inverseMass), body->velocity);
		break;
	case FM_VELOCITY:
		body->velocity = force;
		break;
	}

}

inline void ClearForce(ncBody* body)
{
	body->force = Vector2Zero();
}

void Step(ncBody* body, float timestep);
