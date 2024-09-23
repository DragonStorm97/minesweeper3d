#include "Window.hpp"
#include "minesweeper3d.hpp"
#include "game.hpp"
#include "libminesweeper3d.hpp"
#include <string>
#include <numbers>

// #include <iostream>

#include <cmath>

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

#include <raylib-cpp.hpp>

#include <screens/main_menu.hpp>
#if defined(PLATFORM_WEB)
  #include <emscripten.h>

void RenderLoopCallback(void* arg);

#endif


constexpr static int Height = 600;
constexpr static int Width = 800;
constexpr static int TargetFps = 60;

class Game {
public:

  Main_Menu main_menu;

  std::string title;
  raylib::Window main_window;


  int target_fps = TargetFps;

  Game(int width, int height, std::string game_title, int fps) 
    : title(std::move(game_title)), main_window(width, height, title), target_fps(fps) {
    main_window.SetTargetFPS(target_fps);
  }

  void UpdateDrawFrame() {
    const double time = GetTime();

    BeginDrawing();

    ClearBackground(Color{
      .r = static_cast<unsigned char>((std::sin(time * std::numbers::pi * .25) + 1) / 2 * 0xff),
      .g = static_cast<unsigned char>((std::sin((time + std::numbers::pi) * std::numbers::pi * .25) + 1) / 2 * 0xff),
      .b = static_cast<unsigned char>((std::sin((time + std::numbers::pi / 2) * std::numbers::pi * .25) + 1) / 2 * 0xff),
      .a = 255});

    main_menu.Draw();
    EndDrawing();
  }

  void OuterTick() {
#if defined(PLATFORM_WEB)
  emscripten_set_main_loop_arg(&RenderLoopCallback, this, target_fps, 1);
  // emscripten_set_main_loop(&Game::UpdateDrawFrame, target_fps, 1);
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
  Game game(Width, Height, "Minesweeper3d", TargetFps);
  game.OuterTick();
}

