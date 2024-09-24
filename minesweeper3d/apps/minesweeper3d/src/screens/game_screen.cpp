
#include "screens/game_screen.hpp"
#include "screens/new_game_menu.hpp"
#include <raygui.h>
#include <raylib-cpp.hpp>
#include <screens/main_menu.hpp>
#include <screen_manager.hpp>

void GameScreen::Draw(raylib::Vector2 size)
{
  const raylib::Vector2 buttonSize{size.x/3, size.y/10};

  raylib::DrawText("MINESWEEPER!!!", buttonSize.x, buttonSize.y, 25, raylib::Color::White());
  // TODO: add pause menu button/on escape
  // TODO: add game logic...
  // Snake can reverse direction
  //
}

void GameScreen::CameFrom(Screen* screen) {
  if(screen != nullptr) {
    if(auto* ngm = dynamic_cast<NewGameMenu*>(screen); ngm != nullptr) {
      gridSize = static_cast<int>(ngm->GetGridSize());
      numBombs = static_cast<int>(ngm->GetNumBombs());
      snakeSpeed = static_cast<int>(ngm->GetSnakeSpeed());
      // TODO: reset any other data here, and regenerate board
    }
  }
}
