#include "Functions.hpp"
#include "Vector2.hpp"
#include "raylib.h"
#include "screen.hpp"
#include "screens/new_game_menu.hpp"
#include <raygui.h>
#include <raylib-cpp.hpp>
#include <screens/win_screen.hpp>
#include <screen_manager.hpp>

void WinScreen::Draw(raylib::Vector2 size)
{
  const raylib::Vector2 buttonSize{size.x/4, size.y/10};
  
  raylib::DrawText("You Win!!" , static_cast<int>(size.x/2) - raylib::TextLength("You Win!!")/2, static_cast<int>(size.y/2), 55, raylib::Color::Green());

  const raylib::Vector2 innerSize {size*0.9F};

  if (GuiButton(Rectangle{innerSize.x - buttonSize.x, innerSize.y, buttonSize.x, buttonSize.y}, "New Game") != 0) {
    GetScreenManager()->GoTo<class NewGameMenu>();
  }
}
