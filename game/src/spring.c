#include "spring.h"
#include "body.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

ncSpring_t* ncSprings = NULL;

ncSpring_t* CreateSpring(struct ncBody* body1, struct ncBody* body2, float restLength, float k)
{
	ncSpring_t* spring = (ncSpring_t*)malloc(sizeof(ncSpring_t));
	assert(spring);

	memset(spring, 0, sizeof(ncSpring_t));
	spring->body1 = body1;
	spring->body2 = body2;
	spring->restLength = restLength;
	spring->k = k;

	return spring;
}

void AddSpring(ncSpring_t* spring)
{
	assert(spring);

	spring->prev = NULL;
	spring->next = ncSprings;

	//If list is not empty, update 'prev' of existing head
	if (ncSprings) ncSprings->prev = spring;

	// set head of elements to new elemnt
	ncSprings = spring;
}

void DestroySpring(ncSpring_t* spring)
{
	assert(spring);

	if (spring->prev != NULL) spring->prev->next = spring->next;
	if (spring->next != NULL) spring->next->prev = spring->prev;

	if (spring == ncSprings) ncSprings = spring->next;

	free(spring);
}

void ApplySpringForce(ncSpring_t* springs)
{
	for (ncSpring_t* spring = springs; spring; spring = spring->next)
	{
		Vector2 direction = Vector2Subtract(spring->body2->position,spring->body1->position);
		if (direction.x == 0 && direction.y == 0) continue;

		float length = Vector2Length(direction);
		float x = length - spring->restLength;
		float force = x * spring->k; // k should be negative

		Vector2 ndirection = Vector2Normalize(direction);

		ApplyForce(spring->body1, Vector2Scale(ndirection, force), FM_FORCE);
		ApplyForce(spring->body2, Vector2Scale(ndirection, -force), FM_FORCE);
	}
}


void DestroyAllSprings()
{
	if (!ncSprings) return;

	ncSpring_t* spring = ncSprings;
	ncBody* temp;

	while (spring) {
		temp = spring->next;
		free(spring);
		spring = temp;
	}

	ncSprings = NULL;
}
