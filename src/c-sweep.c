#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "raylib.h"

#define GAME_TITLE "C-Sweep"
#define FPS 60
#define WIDTH 600
#define HEIGHT 600
#define GRID_SIZE 10

typedef enum {
  UNKNOWN = 0,
  OPEN = 1,
  MINE = 2
} MineState;

typedef struct {
  MineState state[GRID_SIZE*GRID_SIZE];
  bool is_first_move;
  bool lost;
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

int mine_index(int row, int col) {
  return row*GRID_SIZE + col;
}

MineState state_at(Game *game, int row, int col) {
  return game->state[mine_index(row, col)];
}

bool is_valid(int row, int col) {
  if (row < 0 || col < 0) {
    return false;
  }
  if (row >= GRID_SIZE || col >= GRID_SIZE) {
    return false;
  }
  return true;
}

int count_adjacent(Game *game, int row, int col) {
  int number_of_mines = 0;
  for(int i = -1; i < 2; i++) {
    for(int j = -1; j < 2; j++) {
      if (i == 0 && j == 0) {
	continue;
      }
      const int dx = row + i;
      const int dy = col + j;
      if (!is_valid(dx, dy)) {
	continue;
      }
      MineState adjacent = state_at(game, dx, dy);
      if (adjacent == MINE) {
	number_of_mines += 1;
      }
    }
  }
  return number_of_mines;
}

void open_cells_with_non_adjacent_mines(Game *game, int row, int col) {
  if (!is_valid(row, col)) {
    return;
  }
  MineState curr = state_at(game, row, col);
  if (curr == MINE || curr == OPEN) {
    return;
  }

  game->state[mine_index(row, col)] = OPEN;
  const int adjacent_mines = count_adjacent(game, row, col);
  if (adjacent_mines > 0) {
    return;
  }
  for(int i = -1; i < 2; i++) {
    for(int j = -1; j < 2; j++) {
      if (i == 0 && j == 0) {
	continue;
      }
      open_cells_with_non_adjacent_mines(game, row + i, col + j);
    }
  }
}

void update_game(Game *game) {
  if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
    Vector2 mouse_pos = GetMousePosition();
    float mine_size = WIDTH / GRID_SIZE;
    int row = mouse_pos.x / mine_size;
    int col = mouse_pos.y / mine_size;
    MineState state = game->state[row*GRID_SIZE + col];
    switch (state) {
    case UNKNOWN: {
      open_cells_with_non_adjacent_mines(game, row, col);
      break;
    }
    case OPEN:
      printf("Hit open! \n");
      break;
    case MINE:
      if (game->is_first_move) {
	
      } else {
	game->lost = true;
      }
    }
    game->is_first_move = false;
  }
}

void generate_mines(Game *game) {
  int number_of_mines = GRID_SIZE * GRID_SIZE * 0.3;
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
  game.is_first_move = true;
  game.lost = false;
  generate_mines(&game);
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);

    update_game(&game);
    if (game.lost) {
      // TODO: Make nice lost screen
      break;
    }
    render_game(game);

    EndDrawing();
  }
  CloseWindow();

  return 0;
}
