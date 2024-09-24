
#include "screens/game_screen.hpp"
#include "Color.hpp"
#include "Rectangle.hpp"
#include "raylib.h"
#include "screens/new_game_menu.hpp"
#include <cstddef>
#include <iostream>
#include <random>
#include <raygui.h>
#include <raylib-cpp.hpp>
#include <screens/main_menu.hpp>
#include <screen_manager.hpp>
#include <unordered_set>

[[nodiscardy]] raylib::Color GetTileColor(std::uint8_t value) {
  return raylib::Color::Blue();
}

void GameScreen::DrawBlock(Coord pos, Block block, int blockSize) {
  constexpr auto textSize = 25;

  switch (block.state) {
    case Block::State::Hidden:
      raylib::Rectangle{static_cast<float>(pos.x), static_cast<float>(pos.y), blockSize * 0.95F, blockSize * 0.95F}.Draw(raylib::Color::Gray());
      return;
    case Block::State::Revealed:
      pos.x += blockSize/10;
      pos.y += blockSize/10;
      if(block.isBomb) {
        raylib::DrawText("X", pos.x, pos.y, textSize, raylib::Color::Red());
      } else if(block.value > 0) {

        raylib::DrawText(TextFormat("%1.0d", block.value), pos.x, pos.y, textSize, GetTileColor(block.value));
      }
      return;
    case Block::State::Snake:
      raylib::Rectangle{static_cast<float>(pos.x), static_cast<float>(pos.y), blockSize * 0.95F, blockSize * 0.95F}.Draw(raylib::Color::SkyBlue());
      return;
    case Block::State::Flagged:
      raylib::DrawText(">", pos.x, pos.y, textSize, raylib::Color::Magenta());
      return;
  }
}

void GameScreen::Draw(raylib::Vector2 size)
{
  const raylib::Vector2 buttonSize{size.x/3, size.y/10};
  raylib::Vector2 offset {size*0.1F};
  const raylib::Vector2 innerSize {size*0.9F};

  // TODO: add pause menu button/on escape
  // TODO: add game logic...
  // Snake can reverse direction
  

  const auto blockSize = static_cast<int>(DrawOffsetGrid(offset, gridSize, innerSize));

  const float halfGrid = blockSize * gridSize * 0.5F;
  offset = ((innerSize + offset) * 0.5F) - raylib::Vector2{halfGrid, halfGrid};

  auto gridSizeSqr = static_cast<std::size_t>(gridSize) * static_cast<std::size_t>(gridSize);
  for(std::size_t blockIdx = 0; blockIdx < gridSizeSqr; ++blockIdx) {
    // if(auto block = blockGrid[blockIdx]; block.value > 0) 
    {
      auto coord = Coord::AsCoord(static_cast<int>(blockIdx), gridSize);
      GameScreen::DrawBlock({static_cast<int>(offset.x + (blockSize * coord.x)), static_cast<int>(offset.y + (blockSize * coord.y))},blockGrid[blockIdx], blockSize);
    }
  }
}

void GameScreen::CameFrom(Screen* screen) {
  if(screen != nullptr) {
    if(auto* ngm = dynamic_cast<NewGameMenu*>(screen); ngm != nullptr) {
      gridSize = static_cast<int>(ngm->GetGridSize());
      numBombs = static_cast<int>(ngm->GetNumBombs());
      snakeSpeed = static_cast<int>(ngm->GetSnakeSpeed());
      snakeMode = ngm->GetSnakeMode();
      GenerateGame();
    }
  }
}

std::unordered_set<Coord, CoordHash> generateUniqueCoordinates(int max, int count) {
  std::unordered_set<Coord, CoordHash> unique_coords;
  unique_coords.reserve(static_cast<std::size_t>(count));

  std::random_device randDevice;
  std::mt19937 gen(randDevice());
  std::uniform_int_distribution<> dist(0, max);

  while (unique_coords.size() < static_cast<std::size_t>(count)) {
    Coord coord = {dist(gen), dist(gen)};
    unique_coords.insert(coord);
  }

  return unique_coords;
}

void GameScreen::GenerateGame() {
  snakeBlocks.clear();

  const auto gridSizeSqr = static_cast<std::size_t>(gridSize * gridSize);
  blockGrid = std::vector<Block>(gridSizeSqr, Block{});

  // just used for while we are generating
  // Generate (unique) Bomb Locations
  auto bombBlocks = generateUniqueCoordinates(gridSize-1, numBombs);

  // Place bombs on grid, and set "radar" values
  for(auto bomb : bombBlocks) {
    auto bombCoord = bomb.As1D(gridSize);
    blockGrid[bombCoord].isBomb = true;
    // Increment grid for surrounding non-bomb cells
    for(int x = -1; x <= 1; ++x) {
      for(int y = -1; y <= 1; ++y) {
        const Coord coord{bomb.x + x, bomb.y + y};
        if(coord.x >= 0 && coord.y >= 0 && coord.x < gridSize && coord.y < gridSize) {
          const auto idx = coord.As1D(gridSize);
          if(auto& cell = blockGrid[idx]; !cell.isBomb) {
            ++cell.value;
          }
        }
      }
    }
  }

  for(int y = 0; y < gridSize; ++y) {
    for(int x = 0; x < gridSize; ++x) {
      std::cout<<static_cast<int>(blockGrid[Coord{x,y}.As1D(gridSize)].value)<< " ";
    }
    std::cout<<std::endl;
  }
}

