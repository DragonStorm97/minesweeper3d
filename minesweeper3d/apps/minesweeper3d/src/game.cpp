#include "Window.hpp"
#include "minesweeper3d.hpp"
#include "game.hpp"
#include "libminesweeper3d.hpp"
#include "screen_manager.hpp"
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

Game::Game(int width, int height, std::string game_title, int fps)
  : title(std::move(game_title)), main_window(width, height, title), target_fps(fps)
{
  GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
  GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, raylib::Color::White());

  main_window.SetTargetFPS(target_fps);
  screenManager.GoTo<MainMenu>();
}

void Game::UpdateDrawFrame() {
  const double time = GetTime();

  main_window.BeginDrawing();

  main_window.ClearBackground(raylib::Color::LightGray());

  screenManager.GetActiveScreen().Draw(main_window.GetSize());
  main_window.EndDrawing();
}

void Game::Start() {
#if defined(PLATFORM_WEB)
emscripten_set_main_loop_arg(&RenderLoopCallback, this, target_fps, 1);
// emscripten_set_main_loop(&Game::UpdateDrawFrame, target_fps, 1);
#else
  bool should_quit = false;
  while (!should_quit) {
    if (main_window.ShouldClose() || IsKeyPressed(KEY_ESCAPE)) {
        should_quit = true;
    }
    UpdateDrawFrame();
  }
#endif
}

Game::~Game() {
  main_window.Close();
}

#if defined (PLATFORM_WEB)
void RenderLoopCallback(void* arg)
{
  static_cast<Game*>(arg)->UpdateDrawFrame();
}
#endif

