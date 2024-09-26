
#include "screens/game_screen.hpp"
#include "Color.hpp"
#include "Mouse.hpp"
#include "Rectangle.hpp"
#include "raylib.h"
#include "screens/new_game_menu.hpp"
#include <cstddef>
#include <iostream>
#include <random>
#include <raygui.h>
#include <raylib-cpp.hpp>
#include <screen_manager.hpp>
#include <unordered_set>

[[nodiscard]] raylib::Color GetTileColor(std::uint8_t value)
{
  switch (value) {
  case 1:
    return raylib::Color::Blue();
  case 2:
    return raylib::Color::Green();
  case 3:
    return raylib::Color::Maroon();
  case 4:
    return raylib::Color::DarkPurple();
  case 5:
    return raylib::Color::Orange();
  case 6:
    return raylib::Color::SkyBlue();
  case 7:
    return raylib::Color::Pink();
  case 8:
    return raylib::Color::Yellow();
  };
  return raylib::Color::Blue();
}

// TODO: only call this when blockSize had changed
int getTextSize(int blockSize)
{
  static int textSize = blockSize;
  while (raylib::MeasureText("X", textSize) > (blockSize - 5)) {
    --textSize;
  }
  return textSize;
}

void GameScreen::DrawBlock(Coord pos, Block block, int blockSize) const
{
  if (static_cast<bool>(block.state & Block::State::Revealed)) {
    pos.x += blockSize / 10;
    pos.y += blockSize / 10;
    if (static_cast<bool>(block.state & Block::State::Snake)) {
      raylib::Rectangle{static_cast<float>(pos.x), static_cast<float>(pos.y), blockSize * 0.95F, blockSize * 0.95F}.Draw(raylib::Color::SkyBlue());
    }
    if (static_cast<bool>(block.state & Block::State::Bomb)) {
      raylib::DrawText("X", pos.x + (blockSize / 2) - (raylib::MeasureText("X", blockTextSize) / 2), pos.y, blockTextSize, raylib::Color::Red());
    } else if (block.value > 0) {
      const char* txt = TextFormat("%1.0d", block.value);
      raylib::DrawText(txt, pos.x + (blockSize / 2) - (raylib::MeasureText(txt, blockTextSize) / 2), pos.y, blockTextSize, GetTileColor(block.value));
    }
    return;
  } else {
    raylib::Rectangle{static_cast<float>(pos.x), static_cast<float>(pos.y), blockSize * 0.95F, blockSize * 0.95F}.Draw(raylib::Color::Gray());
    if (static_cast<bool>(block.state & Block::State::Flagged)) {
      const char* txt = "|>";
      raylib::DrawText(txt, pos.x + (blockSize / 2) - (raylib::MeasureText(txt, blockTextSize) / 2), pos.y, blockTextSize, raylib::Color::Magenta());
      return;
    }
  }
}

void GameScreen::Draw(raylib::Vector2 size, bool wasResized)
{
  const raylib::Vector2 buttonSize{size.x / 3, size.y / 10};
  raylib::Vector2 offset{size * 0.1F};
  const raylib::Vector2 innerSize{size * 0.9F};

  const auto blockSize = static_cast<int>(DrawOffsetGrid(offset, gridSize, innerSize));
  if (wasResized) {
    blockTextSize = getTextSize(blockSize);
  }

  auto gridDim = blockSize * gridSize;
  const float halfGrid = gridDim * 0.5F;
  offset = ((innerSize + offset) * 0.5F) - raylib::Vector2{halfGrid, halfGrid};

  auto gridSizeSqr = static_cast<std::size_t>(gridSize) * static_cast<std::size_t>(gridSize);
  for (std::size_t blockIdx = 0; blockIdx < gridSizeSqr; ++blockIdx) {
    // if(auto block = blockGrid[blockIdx]; block.value > 0)
    {
      auto coord = Coord::AsCoord(static_cast<int>(blockIdx), gridSize);
      DrawBlock({static_cast<int>(offset.x + (blockSize * coord.x)), static_cast<int>(offset.y + (blockSize * coord.y))}, blockGrid[blockIdx], blockSize);
    }
  }

  // TODO: add pause menu button/on escape
  // TODO: add game logic...
  // Snake can reverse direction
  const Rectangle gridRect{offset.x, offset.y, static_cast<float>(gridDim), static_cast<float>(gridDim)};

  if (snakeMode) {
  } else {
    const auto pos = raylib::Mouse::GetPosition();
    if (raylib::Mouse::IsButtonPressed(MOUSE_BUTTON_LEFT)) {
      // std::cout<<"LMB pressed at:"<<raylib::Mouse::GetX()<<","<<raylib::Mouse::GetY()<<std::endl;
      // check to see if we clicked in the grid
      if (pos.x >= offset.x && pos.y >= offset.y && pos.x < (offset.x + (gridDim)) && pos.y < offset.y + (gridDim)) {
        // std::cout<<"CLICKED INSIDE GRID!"<<Coord{static_cast<int>((pos.x - offset.x)/(blockSize)), static_cast<int>((pos.y - offset.y)/(blockSize))}.As1D(gridSize)<<std::endl;
        const Coord gridPos{static_cast<int>((pos.x - offset.x) / (blockSize)), static_cast<int>((pos.y - offset.y) / (blockSize))};
        if (!isGenerated) {
          GenerateGame(gridPos);
          blockTextSize = getTextSize(blockSize);
        }
        RevealFrom(gridPos);
      }
    }
    if (raylib::Mouse::IsButtonPressed(MOUSE_BUTTON_RIGHT)) {
      // check to see if we clicked in the grid
      if (pos.x >= offset.x && pos.y >= offset.y && pos.x < (offset.x + (gridDim)) && pos.y < offset.y + (gridDim)) {
        const Coord gridPos{static_cast<int>((pos.x - offset.x) / (blockSize)), static_cast<int>((pos.y - offset.y) / (blockSize))};
        if (!isGenerated) {
          GenerateGame(gridPos);
          blockTextSize = getTextSize(blockSize);
        }
        auto& block = blockGrid[gridPos.As1D(gridSize)];
        block.state = static_cast<Block::State>(block.state ^ Block::State::Flagged);
      }
    }
  }
}

void GameScreen::RevealFrom(Coord pos)
{
  const auto posIdx = pos.As1D(gridSize);
  if (auto& hitBlock = blockGrid[posIdx]; !static_cast<bool>(hitBlock.state & Block::State::Revealed) && !static_cast<bool>(hitBlock.state & Block::State::Flagged)) {
    hitBlock.state = static_cast<Block::State>(hitBlock.state | Block::State::Revealed);
    if ((hitBlock.state & Block::State::Bomb) != Block::State::Bomb) {
      --numSafeBlocks;
      if (numSafeBlocks == 0) {
        GetScreenManager()->GoTo<class WinScreen>();
      }
      if (hitBlock.value == 0) {
        for (int x = -1; x <= 1; ++x) {
          for (int y = -1; y <= 1; ++y) {
            const Coord coord{pos.x + x, pos.y + y};
            const auto idx = coord.As1D(gridSize);
            if (idx != posIdx && coord.x >= 0 && coord.y >= 0 && coord.x < gridSize && coord.y < gridSize) {
              if (auto& cell = blockGrid[idx]; !static_cast<bool>(cell.state & Block::State::Revealed)) {
                RevealFrom(coord);
              }
            }
          }
        }
      }
    } else {
      GetScreenManager()->GoTo<class LoseScreen>();
    }
  }
}

void GameScreen::CameFrom(Screen* screen)
{
  if (screen != nullptr) {
    if (auto* ngm = dynamic_cast<NewGameMenu*>(screen); ngm != nullptr) {
      gridSize = static_cast<int>(ngm->GetGridSize());
      numBombs = static_cast<int>(ngm->GetNumBombs());
      snakeSpeed = static_cast<int>(ngm->GetSnakeSpeed());
      snakeMode = ngm->GetSnakeMode();
      numSafeBlocks = (gridSize * gridSize) - numBombs;
      isGenerated = false;
      const auto gridSizeSqr = static_cast<std::size_t>(gridSize * gridSize);
      blockGrid = std::vector<Block>(gridSizeSqr, Block{});
    }
  }
}

std::unordered_set<Coord, CoordHash> generateUniqueCoordinates(int max, int count, const std::unordered_set<Coord, CoordHash>& safeBlocks)
{
  std::unordered_set<Coord, CoordHash> unique_coords;
  unique_coords.reserve(static_cast<std::size_t>(count));

  std::random_device randDevice;
  std::mt19937 gen(randDevice());
  std::uniform_int_distribution<> dist(0, max);

  while (unique_coords.size() < static_cast<std::size_t>(count)) {
    Coord coord = {dist(gen), dist(gen)};
    if (!safeBlocks.contains(coord)) {
      unique_coords.insert(coord);
    }
  }

  return unique_coords;
}

void GameScreen::GenerateGame(Coord safeBlock)
{
  snakeBlocks.clear();

  const auto gridSizeSqr = static_cast<std::size_t>(gridSize * gridSize);

  // just used for while we are generating
  // Generate (unique) Bomb Locations
  std::unordered_set<Coord, CoordHash> safeBlocks;
  safeBlocks.insert(safeBlock);
  auto bombBlocks = generateUniqueCoordinates(gridSize - 1, numBombs, safeBlocks);

  // Place bombs on grid, and set "radar" values
  for (auto bomb : bombBlocks) {
    auto bombCoord = bomb.As1D(gridSize);
    blockGrid[bombCoord].state = static_cast<Block::State>(blockGrid[bombCoord].state | Block::State::Bomb);
    // Increment grid for surrounding non-bomb cells
    for (int x = -1; x <= 1; ++x) {
      for (int y = -1; y <= 1; ++y) {
        const Coord coord{bomb.x + x, bomb.y + y};
        if (coord.x >= 0 && coord.y >= 0 && coord.x < gridSize && coord.y < gridSize) {
          const auto idx = coord.As1D(gridSize);
          if (auto& cell = blockGrid[idx]; !static_cast<bool>(cell.state & Block::State::Bomb)) {
            ++cell.value;
          }
        }
      }
    }
  }

  for (int y = 0; y < gridSize; ++y) {
    for (int x = 0; x < gridSize; ++x) {
      std::cout << static_cast<int>(blockGrid[Coord{x, y}.As1D(gridSize)].value) << " ";
    }
    std::cout << std::endl;
  }
  isGenerated = true;
}
