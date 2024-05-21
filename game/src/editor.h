#pragma once
#include "raylib.h"
#include "body.h"

//typedef struct ncEditorData
//{
//	float SliderMinValue;
//	float SliderMaxValue;
//	float SliderGravitationValue;
//} ncEditorData_t;

typedef struct ncEditorData
{
    Vector2 anchor01;

    bool BodyTypeDropdownEditMode;
    int BodyTypeDropdownActive;

    float MassSliderValue;
    float DampingSliderValue;
    float GravityScaleSliderValue;
    float SpringStiffnessSliderValue;
    float RestitutionSliderValue;

    float GravitySliderValue;
    float GravitationSliderValue;
    float TimestepSliderValue;

    bool ResetButtonPressed;
    bool SimulationButtonToggled;
} ncEditorData_t;

extern ncEditorData_t ncEditorData;

extern bool ncEditorActive;
extern bool ncEditorIntersect;

void InitEditor();
void UpdateEditor(Vector2 position);
void DrawEditor(Vector2 position);

struct ncBody* GetBodyIntersect(struct ncBody* bodies, Vector2 position); 
void DrawLineBodyToPosition(struct ncBody* body, Vector2 position);