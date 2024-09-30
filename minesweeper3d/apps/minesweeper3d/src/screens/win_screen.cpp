#include "Functions.hpp"
#include "Vector2.hpp"
#include "raylib.h"
#include "screen.hpp"
#include "screens/new_game_menu.hpp"
#include <raygui.h>
#include <raylib-cpp.hpp>
#include <screen_manager.hpp>
#include <screens/win_screen.hpp>

void WinScreen::Draw(float deltatime, raylib::Vector2 size, bool wasResized)
{
  raylib::DrawText("You Win!!", static_cast<int>(size.x / 2) - raylib::MeasureText("You Win!!", 55) / 2, static_cast<int>(size.y / 3), 55, raylib::Color::Green());

  if (GuiButton(Rectangle{(size.x / 3), (size.y / 3 * 2), size.x / 3, size.y / 10}, "New Game") != 0 || raylib::Keyboard::IsKeyPressed(KEY_ENTER)) {
    GetScreenManager()->GoTo<class NewGameMenu>();
  }
}
