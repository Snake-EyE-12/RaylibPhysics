#include "render.h"
#include "raymath.h"

// Define global variables for screen size, zoom level, and view size
Vector2 ncScreenSize = { 1280, 720 }; // Screen resolution in pixels
float ncScreenZoom = 1.0f; // Zoom level for the view
float ncViewSize = 25.0f; // Size of the view in world units

/**
 * Converts a point from screen coordinates to world coordinates.
 *
 * @param screen The screen coordinates to convert.
 * @return The corresponding world coordinates.
 */
Vector2 ConvertScreenToWorld(Vector2 screen)
{
    // Calculate the aspect ratio of the screen
    float ratio = ncScreenSize.x / ncScreenSize.y;

    // Calculate the extents of the view based on the view size and aspect ratio
    Vector2 extents = (Vector2){ ncViewSize * ratio, ncViewSize };
    extents = Vector2Scale(extents, ncScreenZoom); // Scale the extents by the zoom level

    // Calculate the lower and upper bounds of the view
    Vector2 lower = Vector2Negate(extents); // Negate the extents to get the lower bound
    Vector2 upper = extents; // The upper bound is just the extents

    // Normalize the screen coordinates to the range [0, 1]
    float nx = screen.x / ncScreenSize.x;
    float ny = (ncScreenSize.y - screen.y) / ncScreenSize.y;

    // Convert the normalized screen coordinates to world coordinates
    Vector2 world;
    world.x = ((1 - nx) * lower.x) + (nx * upper.x);
    world.y = ((1 - ny) * lower.y) + (ny * upper.y);

    return world;
}

/**
 * Converts a point from world coordinates to screen coordinates.
 *
 * @param world The world coordinates to convert.
 * @return The corresponding screen coordinates.
 */
Vector2 ConvertWorldToScreen(Vector2 world)
{
    // Calculate the aspect ratio of the screen
    float ratio = ncScreenSize.x / ncScreenSize.y;

    // Calculate the extents of the view based on the view size and aspect ratio
    Vector2 extents = (Vector2){ ncViewSize * ratio, ncViewSize };
    extents = Vector2Scale(extents, ncScreenZoom); // Scale the extents by the zoom level

    // Calculate the lower and upper bounds of the view
    Vector2 lower = Vector2Negate(extents); // Negate the extents to get the lower bound
    Vector2 upper = extents; // The upper bound is just the extents

    // Normalize the world coordinates to the range [0, 1]
    float nx = (world.x - lower.x) / (upper.x - lower.x);
    float ny = (world.y - lower.y) / (upper.y - lower.y);

    // Convert the normalized world coordinates to screen coordinates
    Vector2 screen;
    screen.x = nx * ncScreenSize.x;
    screen.y = (1.0f - ny) * ncScreenSize.y;

    return screen;
}

/**
 * Converts a world coordinate value to its corresponding pixel value on the screen.
 *
 * @param world The world coordinate value to convert.
 * @return The corresponding pixel value.
 */
float ConvertWorldToPixel(float world)
{
    // Calculate the aspect ratio of the screen
    float ratio = ncScreenSize.x / ncScreenSize.y;

    // Calculate the extents of the view based on the view size and aspect ratio
    Vector2 extents = (Vector2){ ncViewSize * ratio, ncViewSize };
    extents = Vector2Scale(extents, ncScreenZoom); // Scale the extents by the zoom level

    // Normalize the world coordinate to the range [0, 1] based on the extent
    float nx = world / extents.x;
    // Convert the normalized world coordinate to a pixel value
    float pixel = nx * ncScreenSize.x;

    return pixel;
}
