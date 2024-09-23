#include "Functions.hpp"
#include "Vector2.hpp"
#include "raylib.h"
#include "screen.hpp"
#include <raygui.h>
#include <raylib-cpp.hpp>
#include <screens/new_game_menu.hpp>
#include <screen_manager.hpp>

void NewGameMenu::Draw(raylib::Vector2 size)
{
  const raylib::Vector2 buttonSize{size.x/4, size.y/10};
  
  raylib::DrawText("New Game Menu" , static_cast<int>(size.y/2) + raylib::TextLength("New Game Menu") /2, static_cast<int>(size.x * 0.05F), 25, raylib::Color::White());

  const raylib::Vector2 offset {size*0.1F};
  const raylib::Vector2 innerSize {size*0.9F};

  DrawOffsetGrid(offset, gridSize, innerSize);

  GuiSlider(Rectangle{ size.x * 0.1F, size.y * 0.15F, buttonSize.x, buttonSize.y/2 }, "Size", TextFormat("%3.0f", gridSize), &gridSize, 3, 100);
  GuiSlider(Rectangle{ size.x * 0.65F, size.y * 0.15F, buttonSize.x, buttonSize.y/2 }, "Number of Bombs", TextFormat("%3.0f", numBombs), &numBombs, 1, (gridSize * gridSize) - gridSize);

  if (GuiButton(Rectangle{innerSize.x - buttonSize.x, innerSize.y, buttonSize.x, buttonSize.y}, "Start Game >") != 0) {
    GetScreenManager()->GoTo<class GameScreen>();
  } else if (GuiButton(Rectangle{offset.x, innerSize.y, buttonSize.x, buttonSize.y}, "< Back to Main Menu") != 0) {
    GetScreenManager()->GoTo<class MainMenu>();
  } 
  gridSize = std::floor(gridSize);
}
