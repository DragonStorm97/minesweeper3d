#pragma once
#include <vector>
#include <memory>

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

class ScreenManager {
private:
  std::vector<std::unique_ptr<Screen>> screens;

public:

};
