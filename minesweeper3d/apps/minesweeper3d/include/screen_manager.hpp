#pragma once

#include <screen.hpp>
#include <screens/main_menu.hpp>
#include <screens/new_game_menu.hpp>
#include <screens/game_screen.hpp>
#include <utility>
#include <memory>

template<typename U, typename... T>
constexpr bool tuple_contains([[maybe_unused]] const std::tuple<T...>& tuple) {
  return (std::is_same_v<U, T> || ...);
}

// The ScreenManager handles switching between screens
class ScreenManager {
private:
  // we add all of our screens here:
  std::tuple<std::unique_ptr<MainMenu>, std::unique_ptr<NewGameMenu>, std::unique_ptr<GameScreen>> screens;

  Screen* activeScreen = nullptr;
public:
  ScreenManager();

  template<typename T>
  void GoTo() {
    if (tuple_contains<std::unique_ptr<T>>(screens)) {
      auto* temp = activeScreen;
      activeScreen = std::get<std::unique_ptr<T>>(screens).get();
      activeScreen->CameFrom(temp);
    }
    else {
      std::unreachable();
    }
  }

  template<typename T>
  [[nodiscard]] const Screen& GetScreen() const {
    if (tuple_contains<std::unique_ptr<T>>(screens)) {
      return std::get<std::unique_ptr<T>>(screens).get();
    }
    else {
      std::unreachable();
    }
  }

  [[nodiscard]] Screen& GetActiveScreen() const {
    return *activeScreen;
  }
};

