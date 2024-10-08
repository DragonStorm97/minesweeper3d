#pragma once

#include <screen.hpp>

class LoseScreen : public Screen {
  public:
  LoseScreen() = default;
  LoseScreen(const LoseScreen&) = default;
  LoseScreen(LoseScreen&&) = delete;
  LoseScreen& operator=(const LoseScreen&) = default;
  LoseScreen& operator=(LoseScreen&&) = delete;
  ~LoseScreen() override = default;

  void Draw(float deltatime, raylib::Vector2 windowSize, bool wasResized) override;

  private:
};
