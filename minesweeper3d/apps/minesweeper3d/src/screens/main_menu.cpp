#include <raygui.h>
#include <raylib-cpp.hpp>
#include <screens/main_menu.hpp>
#include <screen_manager.hpp>

void MainMenu::Draw(raylib::Vector2 size)
{
  const raylib::Vector2 buttonSize{size.x/3, size.y/10};

  raylib::DrawText("Minesweep-Snake", buttonSize.x, buttonSize.y, 25, raylib::Color::White());

  // raylib::DrawText("", int posX, int posY, int fontSize, ::Color color)
  if (GuiButton(Rectangle{buttonSize.x, buttonSize.y * 2.5F, buttonSize.x, buttonSize.y}, "#191#New Game") != 0) {
    GetScreenManager()->GoTo<class NewGameMenu>();
  } 
  // else if (GuiButton(Rectangle{buttonSize.x, (buttonSize.y * 5.0F), buttonSize.x, buttonSize.y}, "#191#Settings") != 0) {
  // }
}
