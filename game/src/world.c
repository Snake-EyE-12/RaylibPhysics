#include "world.h"
#include "body.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

ncBody* ncBodies = NULL;
int ncBodyCount = 0;
Vector2 ncGravity;

ncBody* CreateBody(Vector2 position, float mass, ncBodyType bodyType) 
{
	//Allocate memory for new Body
	ncBody* body = (ncBody*)malloc(sizeof(ncBody));

	//Check if allocation is successful
	assert(body);

	memset(body, 0, sizeof(ncBody));
	body->position = position;
	body->mass = mass;
	body->inverseMass = (bodyType == BT_DYNAMIC) ? 1 / mass : 0; // static and kinematic have no inverse mass
	body->type = bodyType;
	
	return body;
}

void AddBody(ncBody* body)
{
	assert(body);

	//Initialize 'prev' to NULL and 'next' to the head of the list
	body->prev = NULL;
	body->next = ncBodies;

	//If list is not empty, update 'prev' of existing head
	if (ncBodies) ncBodies->prev = body;

	// set head of elements to new elemnt
	ncBodies = body;

	//Increment body count
	ncBodyCount++;

	//Return new Body
	return body;
}

void DestroyBody(ncBody* body)
{
	//Assert if provided Body is not NULL
	assert(body);

	//If 'prev' is not NULL, set 'prev->next' to 'body->next'
	if (body->prev != NULL) body->prev->next = body->next;

	//If 'next' is not NULL, set 'next->prev' to 'body->prev'
	if (body->next != NULL) body->next->prev = body->prev;

	//If body is the head, update head to 'body->next'
	if (body == ncBodies) ncBodies = body->next;

	//Decrement body count
	ncBodyCount--;
	
	//Free the body
	free(body);
}