#pragma once

#include "Vector2.hpp"
#include <screen.hpp>

class MainMenu : public Screen {
public:
  MainMenu() = default;
  MainMenu(const MainMenu&) = default;
  MainMenu(MainMenu&&) = delete;
  MainMenu& operator=(const MainMenu&) = default;
  MainMenu& operator=(MainMenu&&) = delete;
  ~MainMenu() override = default;

  void Draw(raylib::Vector2 windowSize) override;

};
