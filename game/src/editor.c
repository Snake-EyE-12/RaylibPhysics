#include "editor.h"
#define RAYGUI_IMPLEMENTATION
#include "../../raygui/src/raygui.h"
#include "render.h"

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

    ncEditorData.anchor01 = (Vector2){ 1500, 48 };

    ncEditorData.WorldGroupBoxActive = true;
    ncEditorData.BodyTypeDropdownEditMode = false;
    ncEditorData.BodyTypeDropdownActive = 2;
    ncEditorData.MassMinSliderValue = 1.0f;
    ncEditorData.GravitationSliderValue = 5.0f;
    ncEditorData.MassMaxSliderValue = 3.0f;
    ncEditorData.DampingSliderValue = 0.0f;
    ncEditorData.GravityScaleSliderValue = 0.0f;

    editorRect = (Rectangle){ ncEditorData.anchor01.x + 0, ncEditorData.anchor01.y + 0, 300, 600 };

}

void UpdateEditor(Vector2 position)
{
    
}

void DrawEditor(Vector2 position)
{
    if (ncEditorData.BodyTypeDropdownEditMode) GuiLock();

    if (ncEditorData.WorldGroupBoxActive)
    {
        ncEditorData.WorldGroupBoxActive = !GuiWindowBox((Rectangle) { ncEditorData.anchor01.x + 16, ncEditorData.anchor01.y + 280, 272, 256 }, "World");
        GuiGroupBox((Rectangle) { ncEditorData.anchor01.x + 0, ncEditorData.anchor01.y + 0, 312, 624 }, "Editor");
        GuiGroupBox((Rectangle) { ncEditorData.anchor01.x + 16, ncEditorData.anchor01.y + 40, 272, 224 }, "Body");
        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 104, ncEditorData.anchor01.y + 96, 168, 24 }, "Mass Min", NULL, & ncEditorData.MassMinSliderValue, 1, 10);
        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 104, ncEditorData.anchor01.y + 320, 168, 24 }, "Gravitation", NULL, & ncEditorData.GravitationSliderValue, 0, 30);
        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 104, ncEditorData.anchor01.y + 136, 168, 24 }, "Mass Max", NULL, & ncEditorData.MassMaxSliderValue, 1, 10);
        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 104, ncEditorData.anchor01.y + 176, 168, 24 }, "Damping", NULL, & ncEditorData.DampingSliderValue, 0, 5);
        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 104, ncEditorData.anchor01.y + 216, 168, 24 }, "Gravity Scale", NULL, & ncEditorData.GravityScaleSliderValue, 0, 10);
        if (GuiDropdownBox((Rectangle) { ncEditorData.anchor01.x + 32, ncEditorData.anchor01.y + 60, 240, 24 }, "STATIC;KINEMATIC;DYNAMIC", & ncEditorData.BodyTypeDropdownActive, ncEditorData.BodyTypeDropdownEditMode)) ncEditorData.BodyTypeDropdownEditMode = !ncEditorData.BodyTypeDropdownEditMode;
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
