#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"

#define GAME_TITLE "C-Sweep"
#define FPS 60
#define WIDTH 600
#define HEIGHT 600
#define GRID_SIZE 60

typedef enum {
  UNKNOWN,
  OPEN,
  MINE
} MineState;

typedef struct {
  MineState state[GRID_SIZE*GRID_SIZE];
} Game;

void render_game(Game game) {
  const float width = GetScreenWidth();
  const float mine_size = width / GRID_SIZE;
  const float padding = 1;
  for(size_t row = 0; row < GRID_SIZE; row++) {
    for(size_t col = 0; col < GRID_SIZE; col++) {
      const MineState state = game.state[row*GRID_SIZE + col];
      DrawRectangle(
	  row * mine_size + padding,
	  col * mine_size + padding,
	  mine_size - padding * 2,
	  mine_size - padding * 2,
	  state == MINE ? RED : WHITE
      );
    }
  }
}

void generate_mines(Game *game) {
  int number_of_mines = 100;
  while (number_of_mines > 0) {
    const int row = rand() % GRID_SIZE;
    const int col = rand() % GRID_SIZE;
    const int index = row * GRID_SIZE + col;
    if (game->state[index] == UNKNOWN) {
      game->state[index] = MINE;
    }
    number_of_mines--;
  }
}

int main() {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(WIDTH, HEIGHT, GAME_TITLE);
  SetWindowMinSize(200, 400);
  SetTargetFPS(FPS);

  Game game = {0};
  generate_mines(&game);
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    render_game(game);
    EndDrawing();
  }

  return 0;
}
