#include "game.hpp"
#include "Color.hpp"
#include "Functions.hpp"
#include "Window.hpp"
#include "libminesweeper3d.hpp"
#include "minesweeper3d.hpp"
#include "raylib.h"
#include "screen_manager.hpp"
#include "screens/new_game_menu.hpp"
#include <string>

#include <cmath>

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

#include <raylib-cpp.hpp>

#if defined(PLATFORM_WEB)
#include <emscripten.h>
#include <emscripten/html5.h>
void RenderLoopCallback(void* arg);
#endif

Game::Game(int width, int height, std::string game_title, int fps)
    : title(std::move(game_title)), main_window(width, height, title, FLAG_WINDOW_RESIZABLE), target_fps(fps), screenWidth(width), screenHeight(height)
{
  GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
  GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, raylib::Color::White());
  main_window.SetTargetFPS(target_fps);
  screenManager.GoTo<NewGameMenu>();
}

void Game::UpdateDrawFrame()
{
  static auto windowSize = main_window.GetSize();

  const float deltatime = GetFrameTime();

  main_window.BeginDrawing();

  main_window.ClearBackground(raylib::Color::LightGray());

  if (windowSize != main_window.GetSize()) {
#if defined(PLATFORM_WEB)
    double cssW = 0;
    double cssH = 0;
    emscripten_get_element_css_size("#canvas", &cssW, &cssH);
    main_window.SetSize({static_cast<float>(cssW), static_cast<float>(cssH)});
#endif
  }

  // raylib::DrawText(TextFormat("%1.3fs", deltatime), 0, 0, 40, raylib::Color::Pink());
  screenManager.GetActiveScreen().Draw(deltatime, main_window.GetSize(), main_window.IsResized());

  main_window.EndDrawing();
}

void Game::Start()
{
#if defined(PLATFORM_WEB)
  double cssW = 0;
  double cssH = 0;
  emscripten_get_element_css_size("#canvas", &cssW, &cssH);
  main_window.SetSize({static_cast<float>(cssW), static_cast<float>(cssH)});
  emscripten_set_main_loop_arg(&RenderLoopCallback, this, target_fps, 1);
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

Game::~Game()
{
  main_window.Close();
}

#if defined(PLATFORM_WEB)
void RenderLoopCallback(void* arg)
{
  static_cast<Game*>(arg)->UpdateDrawFrame();
}
#endif
