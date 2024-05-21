#include "editor.h"
#define RAYGUI_IMPLEMENTATION
#include "../../raygui/src/raygui.h"
#include "render.h"

#define EDITOR_DATA(data) TextFormat("%0.2f", data), &data

bool ncEditorActive = true;
bool ncEditorIntersect = false;
Rectangle editorRect;

Texture2D cursorTexture;

// GUI VARS
//bool EditorBoxActive = true;
ncEditorData_t ncEditorData;
Vector2 anchor01 = { 1600, 40 };

bool WorldGroupBoxActive = true;
bool BodyTypeDropdownEditMode = false;
int BodyTypeDropdownActive = 0;
float MassMinSliderValue = 0.0f;
float GravitationSliderValue = 0.0f;
float MassMaxSliderValue = 0.0f;
float DampingSliderValue = 0.0f;
float GravityScaleSliderValue = 0.0f;

void InitEditor()
{
    GuiLoadStyle("raygui/styles/cyber/style_cyber.rgs");
    Image image = LoadImage("resources/reticle.png");
    cursorTexture = LoadTextureFromImage(image);
    UnloadImage(image);
    HideCursor();

    ncEditorData.anchor01 = (Vector2){ 1500, 25 };

    ncEditorData.BodyTypeDropdownEditMode = false;
    ncEditorData.BodyTypeDropdownActive = 2;

    ncEditorData.MassSliderValue = 2.0f;
    ncEditorData.DampingSliderValue = 0.0f;
    ncEditorData.GravityScaleSliderValue = 1.0f;
    ncEditorData.SpringStiffnessSliderValue = 10.0f;
    ncEditorData.RestitutionSliderValue = 0.4f;

    ncEditorData.GravitySliderValue = 0.0f;
    ncEditorData.GravitationSliderValue = 0.0f;
    ncEditorData.TimestepSliderValue = 50.0f;

    ncEditorData.ResetButtonPressed = false;
    ncEditorData.SimulationButtonToggled = true;

    editorRect = (Rectangle){ ncEditorData.anchor01.x + 0, ncEditorData.anchor01.y + 0, 350, 725 };

}

void UpdateEditor(Vector2 position)
{ 
    if (IsKeyPressed(KEY_TAB)) ncEditorActive = !ncEditorActive;

    ncEditorIntersect = ncEditorActive && CheckCollisionPointRec(position, editorRect);
}

void DrawEditor(Vector2 position)
{
    if (ncEditorData.BodyTypeDropdownEditMode) GuiLock();

    if (ncEditorActive)
    {
        GuiGroupBox(editorRect, "Editor");
        GuiGroupBox((Rectangle) { ncEditorData.anchor01.x + 15, ncEditorData.anchor01.y + 15, 320, 325 }, "Body");
        GuiGroupBox((Rectangle) { ncEditorData.anchor01.x + 15, ncEditorData.anchor01.y + 380, 320, 250 }, "World");

        if (GuiDropdownBox((Rectangle) { ncEditorData.anchor01.x + 104, ncEditorData.anchor01.y + 45, 168, 24 }, "STATIC;KINEMATIC;DYNAMIC", & ncEditorData.BodyTypeDropdownActive, ncEditorData.BodyTypeDropdownEditMode)) ncEditorData.BodyTypeDropdownEditMode = !ncEditorData.BodyTypeDropdownEditMode;

        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 104, ncEditorData.anchor01.y + 160, 168, 25 }, "Mass", EDITOR_DATA(ncEditorData.MassSliderValue), 1, 10);
        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 104, ncEditorData.anchor01.y + 195, 168, 25 }, "Damping", EDITOR_DATA(ncEditorData.DampingSliderValue), 0, 5);
        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 104, ncEditorData.anchor01.y + 230, 168, 25 }, "Gravity Scale", EDITOR_DATA(ncEditorData.GravityScaleSliderValue), 0, 10);
        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 104, ncEditorData.anchor01.y + 265, 168, 25 }, "Stiffness", EDITOR_DATA(ncEditorData.SpringStiffnessSliderValue), 0, 30);
        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 104, ncEditorData.anchor01.y + 300, 168, 25 }, "Restitution", EDITOR_DATA(ncEditorData.RestitutionSliderValue), 0, 1);


        GuiSlider((Rectangle) { ncEditorData.anchor01.x + 104, ncEditorData.anchor01.y + 425, 168, 25 }, "Gravity", EDITOR_DATA(ncEditorData.GravitySliderValue), -20, 20);
        GuiSlider((Rectangle) { ncEditorData.anchor01.x + 104, ncEditorData.anchor01.y + 460, 168, 25 }, "Gravitation", EDITOR_DATA(ncEditorData.GravitationSliderValue), 0, 30);
        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 104, ncEditorData.anchor01.y + 495, 168, 25 }, "Timestep", EDITOR_DATA(ncEditorData.TimestepSliderValue), 0, 100);
    
        ncEditorData.ResetButtonPressed = GuiButton((Rectangle) { ncEditorData.anchor01.x + 52, ncEditorData.anchor01.y + 650, 100, 35 }, "Reset");
        GuiToggle((Rectangle) { ncEditorData.anchor01.x + 176, ncEditorData.anchor01.y + 650, 100, 35 }, "Simulate", &ncEditorData.SimulationButtonToggled);
    
    }

    DrawTexture(cursorTexture, (int)position.x - cursorTexture.width * 0.5f, (int)position.y - cursorTexture.height * 0.5f, WHITE);
    GuiUnlock();
}

ncBody* GetBodyIntersect(ncBody* bodies, Vector2 position)
{
    for (ncBody* body = bodies; body; body = body->next)
    {
        Vector2 screen = ConvertWorldToScreen(body->position);
        if (CheckCollisionPointCircle(position, screen, ConvertWorldToPixel(body->mass)))
        {
            return body;
        }
    }

    return NULL;
}

void DrawLineBodyToPosition(ncBody* body, Vector2 position)
{
    Vector2 screen = ConvertWorldToScreen(body->position);
    DrawLine((int)screen.x, (int)screen.y, (int)position.x - cursorTexture.width / 2, (int)position.y - cursorTexture.height / 2, YELLOW);
}
