#pragma once
#include <raylib-cpp.hpp>
#include <screen_manager.hpp>
#include <string>

class Game {
  public:
  ScreenManager screenManager;

  std::string title;
  raylib::Window main_window;
  int target_fps;
  int screenWidth;
  int screenHeight;

  Game(const Game&) = delete;
  Game(Game&&) = delete;
  Game& operator=(const Game&) = delete;
  Game& operator=(Game&&) = delete;

  Game(int width, int height, std::string game_title, int fps);

  void UpdateDrawFrame();

  void Start();

  ~Game();
};
