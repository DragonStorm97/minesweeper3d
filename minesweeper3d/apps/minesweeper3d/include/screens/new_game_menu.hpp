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

  void Draw(raylib::Vector2 windowSize) override;

  [[nodiscard]] float GetGridSize() const {
    return gridSize;
  }
  [[nodiscard]] float GetNumBombs() const {
    return numBombs;
  }
private:
  float gridSize = 3;
  float numBombs = 1;

};
