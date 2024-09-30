#include "Functions.hpp"
#include "Vector2.hpp"
#include "raylib.h"
#include "screen.hpp"
#include <raygui.h>
#include <raylib-cpp.hpp>
#include <screen_manager.hpp>
#include <screens/new_game_menu.hpp>

void NewGameMenu::Draw(float deltatime, raylib::Vector2 size, bool wasResized)
{
  const raylib::Vector2 buttonSize{size.x / 4, size.y / 10};

  raylib::DrawText("New Game", static_cast<int>(size.x / 2) - raylib::MeasureText("New Game", 25) / 2, static_cast<int>(size.y * 0.02F), 25, raylib::Color::White());

  const raylib::Vector2 offset{size * 0.1F};
  const raylib::Vector2 innerSize{size * 0.9F};

  [[maybe_unused]] auto blockSize = static_cast<int>(DrawOffsetGrid(offset, gridSize, innerSize));

  GuiSlider(Rectangle{offset.x, offset.y, buttonSize.x / 3 * 2, buttonSize.y / 2}, "Size", TextFormat("%3.0f", gridSize), &gridSize, 3, 100);
  GuiSlider(Rectangle{size.x * 0.75F, offset.y, buttonSize.x / 3 * 2, buttonSize.y / 2}, "Bombs", TextFormat("%3.0f", numBombs), &numBombs, 1, (gridSize * gridSize) - gridSize);
  if (snakeMode) {
    GuiSlider(Rectangle{size.x * 0.45F, offset.y, buttonSize.x / 3 * 2, buttonSize.y / 2}, "Speed", TextFormat("%3.0f", snakeSpeed), &snakeSpeed, 0, 10);
  }

  if (GuiButton(Rectangle{innerSize.x - buttonSize.x, innerSize.y, buttonSize.x, buttonSize.y}, "Start Game >") != 0) {
    GetScreenManager()->GoTo<class GameScreen>();
  }

  GuiToggle(raylib::Rectangle{offset.x, size.x * 0.02F, buttonSize.x / 3 * 2, buttonSize.y / 2}, "Snake Mode?", &snakeMode);
  numBombs = std::floor(numBombs);
  snakeSpeed = std::floor(snakeSpeed);
  gridSize = std::floor(gridSize);
}
