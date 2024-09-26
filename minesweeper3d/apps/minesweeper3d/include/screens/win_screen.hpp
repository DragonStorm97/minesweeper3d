#pragma once

#include <screen.hpp>

class WinScreen : public Screen {
public:
  WinScreen() = default;
  WinScreen(const WinScreen&) = default;
  WinScreen(WinScreen&&) = delete;
  WinScreen& operator=(const WinScreen&) = default;
  WinScreen& operator=(WinScreen&&) = delete;
  ~WinScreen() override = default;

  void Draw(raylib::Vector2 windowSize, bool wasResized) override;
private:
};
