#include "minesweeper3d.hpp"
#include "game.hpp"
#include "libminesweeper3d.hpp"
// #include <string>
// #include <iostream>

#include <cmath>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <screens/main_menu.hpp>
#if defined(PLATFORM_WEB)
  #include <emscripten.h>

void RenderLoopCallback(void* arg);

#endif


class Game {
public:
  Main_Menu main_menu;

  void Init() {
    InitWindow(640, 480, "Minesweeper 3D!");
    SetTargetFPS(60);
  }

  void UpdateDrawFrame() {
    const double time = GetTime();

    BeginDrawing();

    ClearBackground(Color{
      .r = static_cast<unsigned char>((sin(time * 3.14 * .25) + 1) / 2 * 0xff),
      .g = static_cast<unsigned char>((sin((time + 3.14) * 3.14 * .25) + 1) / 2 * 0xff),
      .b = static_cast<unsigned char>((sin((time + 3.14 / 2) * 3.140 * .25) + 1) / 2 * 0xff),
      .a = 255});

    main_menu.Draw();
    EndDrawing();
  }

  void OuterTick() {
#if defined(PLATFORM_WEB)
  emscripten_set_main_loop_arg(&RenderLoopCallback, this, 60, 1);
  // emscripten_set_main_loop(&Game::UpdateDrawFrame, 60, 1);
#else
    bool should_quit = false;
    while (!should_quit) {
      if (WindowShouldClose() || IsKeyPressed(KEY_ESCAPE)) {
          should_quit = true;
      }
      UpdateDrawFrame();
    }
#endif
  }

  ~Game() {
    CloseWindow();
  }
};

#if defined (PLATFORM_WEB)
void RenderLoopCallback(void* arg)
{
  static_cast<Game*>(arg)->UpdateDrawFrame();
}
#endif

int main()
{
  Game game;
  game.Init();
  game.OuterTick();


}

