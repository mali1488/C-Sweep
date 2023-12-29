#include <stdio.h>
#include "raylib.h"

#define GAME_TITLE "C-Sweep"
#define FPS 60

int main() {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(450, 800, GAME_TITLE);
  SetWindowMinSize(200, 400);
  SetTargetFPS(FPS);
    
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    EndDrawing();
  }

  return 0;
}
