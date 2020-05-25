#include "raylib.h"
#include <stdio.h>

#define TILE_WIDTH (128)
#define TILE_HEIGHT (64)
#define TILE_WIDTH_HALF (64)
#define TILE_HEIGHT_HALF (32)

typedef struct tile tile;
struct tile
{
    Vector2 pos;
    bool isSelected;
};

Vector2 mapToScreen(Vector2 map)
{
    return (Vector2){ (map.x - map.y) * TILE_WIDTH_HALF,
                      (map.x + map.y) * TILE_HEIGHT_HALF };
}

Vector2 screenToMap(Vector2 screen)
{
    return (Vector2){ (screen.x / TILE_WIDTH_HALF + screen.y / TILE_HEIGHT_HALF) /2,
                      (screen.y / TILE_HEIGHT_HALF -(screen.x / TILE_WIDTH_HALF)) /2 };
}

int main(void)
{
    InitWindow(800, 600, "Raylib playground");

    tile map[5][5] = { 0 };
    bool drawIsometric = false;
    Camera2D camera = { 0 };
    camera.zoom = 1.0f;
    camera.offset = (Vector2){ 400, 300 };

    Texture2D tile = LoadTexture("../assets/tile_1.png");
    Texture2D tile_selected = LoadTexture("../assets/tile_2.png");

    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
        {
            map[i][j].pos = (Vector2){ i, j };
            map[i][j].isSelected = false;
        }

    while (!WindowShouldClose())
    {
        // Update view
        if (IsKeyPressed(KEY_SPACE))
        {
            drawIsometric = !drawIsometric;

            if (drawIsometric) camera.target = mapToScreen(map[0][0].pos);
            else camera.target = (Vector2){ 0 , 0 };
        }

        // check hover
        Vector2 mouse_pos = screenToMap(GetScreenToWorld2D(GetMousePosition(), camera));

        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
            {
                Rectangle tile = { map[i][j].pos.x, map[i][j].pos.y, 1, 1 };
                if (CheckCollisionPointRec(mouse_pos, tile))
                    map[i][j].isSelected = true;
                else if (map[i][j].isSelected)
                    map[i][j].isSelected = false;
            }

        BeginDrawing();

            ClearBackground(BLACK);

            BeginMode2D(camera);

                if (drawIsometric)
                {
                    // Draw isometric map
                    for (int i = 0; i < 4; i++)
                        for (int j = 0; j < 4; j++)
                        {
                            Vector2 start, end;
                            start   = mapToScreen(map[i][j].pos);

                            // Draw Texture
                            Texture2D* texture = (map[i][j].isSelected) ? &tile_selected : &tile;
                            // DrawTexture(tile, start.x + TILE_HEIGHT_HALF, start.y - TILE_WIDTH_HALF, WHITE);
                            DrawTextureEx((*texture), (Vector2){ start.x - TILE_WIDTH_HALF , start.y }, 0.0f, 4.0f, WHITE);

                            end     = mapToScreen(map[i][j + 1].pos);
                            DrawLine(start.x, start.y, end.x, end.y, RED);

                            end     = mapToScreen(map[i + 1][j].pos);
                            DrawLine(start.x, start.y, end.x, end.y, RED);

                        }

                        Vector2 start, end;

                        start   = mapToScreen(map[0][4].pos);
                        end     = mapToScreen(map[4][4].pos);
                        DrawLine(start.x, start.y, end.x, end.y, RED);

                        start     = mapToScreen(map[4][0].pos);
                        DrawLine(start.x, start.y, end.x, end.y, RED);
                }
                else
                {
                    // Draw normal map
                    for (int i = 0; i < 4; i++)
                        for (int j = 0; j < 4; j++)
                        {
                            Vector2 start, end;

                            start   = (Vector2){ map[i][j].pos.x * TILE_HEIGHT, map[i][j].pos.y * TILE_HEIGHT };
                            end     = (Vector2){ (map[i][j + 1].pos.x) * TILE_HEIGHT, (map[i][j + 1].pos.y) * TILE_HEIGHT};
                            DrawLine(start.x, start.y, end.x, end.y, RED);

                            end     = (Vector2){ (map[i + 1][j].pos.x) * TILE_HEIGHT, (map[i + 1][j].pos.y) * TILE_HEIGHT};
                            DrawLine(start.x, start.y, end.x, end.y, RED);
                        }

                    // Draw the last lines
                    DrawLine(map[0][4].pos.x * TILE_HEIGHT, map[0][4].pos.y * TILE_HEIGHT, map[4][4].pos.x * TILE_HEIGHT, map[4][4].pos.y * TILE_HEIGHT, RED);
                    DrawLine(map[4][0].pos.x * TILE_HEIGHT, map[4][0].pos.y * TILE_HEIGHT, map[4][4].pos.x * TILE_HEIGHT, map[4][4].pos.y * TILE_HEIGHT, RED);
                }

            EndMode2D();

            Vector2 realMousePos = GetScreenToWorld2D(GetMousePosition(), camera);

            char text[25];
            sprintf(text, "Screen Pos: X: %f, Y: %f", realMousePos.x, realMousePos.y);
            DrawText(text, 10, 10, 10, RED);

            sprintf(text, "Map Pos: X: %f, Y: %f", mouse_pos.x, mouse_pos.y);
            DrawText(text, 10, 25, 10, RED);

        EndDrawing();
    }

    return 0;
}