#pragma once

#include <raylib-cpp.hpp>


// Screens are essentialy "levels", they encapsulate a view as well as the logic for the view
class Screen {
public:
  Screen() = default;
  Screen(const Screen&) = default;
  Screen(Screen&&) = delete;
  Screen& operator=(const Screen&) = default;
  Screen& operator=(Screen&&) = delete;
  virtual ~Screen() = default;

  virtual void Init(class ScreenManager* manager) {
    screenManager = manager;
  };

  virtual void Draw(raylib::Vector2 windowSize) = 0;

  [[nodiscard]] class ScreenManager* GetScreenManager() const {
    return screenManager;
  }

  virtual void CameFrom([[maybe_unused]] Screen* screen) {};

private:
  // ScreenManager handles the lifetime of Screens, so they are gaurenteed to outlive the screens
  class ScreenManager* screenManager = nullptr;

};

inline float DrawOffsetGrid(raylib::Vector2 offset, float gridSize, raylib::Vector2 innerSize) {
  
  const float square_size = std::min(std::min(innerSize.x, innerSize.y)/gridSize, 30.0F);
  const float halfGrid = square_size * gridSize * 0.5F;
  offset = ((innerSize + offset) * 0.5F) - raylib::Vector2{halfGrid, halfGrid};

  // Draw grid lines
  for (int i = 0; i < gridSize+1; i++) {
    DrawLineV(Vector2{square_size*i + offset.x, offset.y}, Vector2{square_size*i + offset.x, square_size*gridSize + offset.y}, raylib::Color::Black());
    DrawLineV(Vector2{offset.x, square_size*i + offset.y}, Vector2{square_size * gridSize + offset.x, square_size*i + offset.y}, raylib::Color::Black());
  }

  return square_size;
}
