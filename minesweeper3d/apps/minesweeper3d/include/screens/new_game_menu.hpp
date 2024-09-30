#pragma once

#include "Vector2.hpp"
#include <screen.hpp>

class NewGameMenu : public Screen {
  public:
  NewGameMenu() = default;
  NewGameMenu(const NewGameMenu&) = default;
  NewGameMenu(NewGameMenu&&) = delete;
  NewGameMenu& operator=(const NewGameMenu&) = default;
  NewGameMenu& operator=(NewGameMenu&&) = delete;
  ~NewGameMenu() override = default;

  void Draw(float deltatime, raylib::Vector2 windowSize, bool wasResized) override;

  [[nodiscard]] float GetGridSize() const
  {
    return gridSize;
  }
  [[nodiscard]] float GetNumBombs() const
  {
    return numBombs;
  }
  [[nodiscard]] float GetSnakeSpeed() const
  {
    return snakeSpeed;
  }
  [[nodiscard]] bool GetSnakeMode() const
  {
    return snakeMode;
  }

  private:
  float gridSize = 3;
  float numBombs = 1;
  // the speed of the snake, in blocks/s
  float snakeSpeed = 1;
  bool snakeMode = false;
};
