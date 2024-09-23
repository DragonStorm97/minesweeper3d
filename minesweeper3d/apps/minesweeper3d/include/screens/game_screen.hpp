#pragma once

#include "Vector2.hpp"
#include <screen.hpp>

class GameScreen : public Screen {
public:
  GameScreen() = default;
  GameScreen(const GameScreen&) = default;
  GameScreen(GameScreen&&) = delete;
  GameScreen& operator=(const GameScreen&) = default;
  GameScreen& operator=(GameScreen&&) = delete;
  ~GameScreen() override = default;

  void Draw(raylib::Vector2 windowSize) override;

  void CameFrom(Screen* screen) override;

private:

  int gridSize = 3;
  int numBombs = 1;

};

