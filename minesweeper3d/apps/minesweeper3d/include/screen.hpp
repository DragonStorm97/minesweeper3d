#pragma once

class Screen {
public:
  Screen() = default;
  Screen(const Screen&) = default;
  Screen(Screen&&) = delete;
  Screen& operator=(const Screen&) = default;
  Screen& operator=(Screen&&) = delete;
  virtual ~Screen() = default;

  virtual void Draw() = 0;
};
