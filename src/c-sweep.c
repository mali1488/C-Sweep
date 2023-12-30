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

Color color_for_state(MineState state) {
  switch (state) {
  case UNKNOWN: return WHITE;
  case OPEN: return GREEN;
  case MINE: return RED;
  }
}

void render_game(Game game) {
  const float mine_size = WIDTH / GRID_SIZE;
  const float padding = 1;
  for(size_t row = 0; row < GRID_SIZE; row++) {
    for(size_t col = 0; col < GRID_SIZE; col++) {
      const MineState state = game.state[row*GRID_SIZE + col];
      Rectangle rec = {
	  .x = row * mine_size + padding,
	  .y = col * mine_size + padding,
	  .width = mine_size - padding * 2,
	  .height = mine_size - padding * 2,
      };
      DrawRectangleRec(rec, color_for_state(state));
    }
  }
}

Rectangle get_mine_rectangle(int x, int y) {
  const float mine_size = WIDTH / GRID_SIZE;
  Rectangle rect = {
    .x = x,
    .y = y,
    .width = mine_size,
    .height = mine_size
  };
  return rect;
}

void update_game(Game *game) {
  if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
    Vector2 mouse_pos = GetMousePosition();
    float mine_size = WIDTH / GRID_SIZE;
    int row = mouse_pos.x / mine_size;
    int col = mouse_pos.y / mine_size;
    game->state[row*GRID_SIZE + col] = OPEN;
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

    update_game(&game);
    render_game(game);

    EndDrawing();
  }

  return 0;
}
