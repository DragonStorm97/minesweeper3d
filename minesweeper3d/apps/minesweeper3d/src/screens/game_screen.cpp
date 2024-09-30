
#include "screens/game_screen.hpp"
#include "Color.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "Rectangle.hpp"
#include "Vector2.hpp"
#include "raylib.h"
#include "screens/lose_screen.hpp"
#include "screens/new_game_menu.hpp"
#include <cstddef>
#include <random>
#include <raygui.h>
#include <raylib-cpp.hpp>
#include <screen_manager.hpp>
#include <unordered_set>

[[nodiscard]] constexpr Color GetTileColor(std::uint8_t value)
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
    // pos.x += blockSize / 50;
    // pos.y += blockSize / 50;
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

void GameScreen::Draw(float deltatime, raylib::Vector2 size, bool wasResized)
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
  offset = (size * 0.5F) - raylib::Vector2{halfGrid, halfGrid};

  auto gridSizeSqr = static_cast<std::size_t>(gridSize) * static_cast<std::size_t>(gridSize);
  for (std::size_t blockIdx = 0; blockIdx < gridSizeSqr; ++blockIdx) {
    auto coord = Coord::AsCoord(static_cast<int>(blockIdx), gridSize);
    DrawBlock({static_cast<int>(offset.x + (blockSize * coord.x)), static_cast<int>(offset.y + (blockSize * coord.y))}, blockGrid[blockIdx], blockSize);
  }

  if (raylib::Keyboard::IsKeyPressed(KEY_Q)) {
    GetScreenManager()->GoTo<LoseScreen>();
    return;
  }

  // TODO: Snake can reverse direction?

  if (snakeMode) {
    if (raylib::Keyboard::IsKeyPressed(KEY_UP) || raylib::Keyboard::IsKeyPressed(KEY_W)) {
      SnakeDirection.y = -1;
      SnakeDirection.x = 0;
    } else if (raylib::Keyboard::IsKeyPressed(KEY_DOWN) || raylib::Keyboard::IsKeyPressed(KEY_S)) {
      SnakeDirection.y = 1;
      SnakeDirection.x = 0;
    } else if (raylib::Keyboard::IsKeyPressed(KEY_LEFT) || raylib::Keyboard::IsKeyPressed(KEY_A)) {
      SnakeDirection.x = -1;
      SnakeDirection.y = 0;
    } else if (raylib::Keyboard::IsKeyPressed(KEY_RIGHT) || raylib::Keyboard::IsKeyPressed(KEY_D)) {
      SnakeDirection.x = 1;
      SnakeDirection.y = 0;
    } else if (snakeSpeed == 0) {
      SnakeDirection.x = 0;
      SnakeDirection.y = 0;
    }

    if (!isGenerated) {
      snakePosition = raylib::Vector2{static_cast<float>(gridSize / 2), static_cast<float>(gridSize / 2)};
      GenerateGame(Coord::FromVector2(snakePosition));
      snakeBlocks.emplace_front(Coord::FromVector2(snakePosition));
      blockTextSize = getTextSize(blockSize);
      auto& frontSnakeBlock = blockGrid[Coord::FromVector2(snakePosition).As1D(gridSize)];
      frontSnakeBlock.state = static_cast<Block::State>(frontSnakeBlock.state | Block::State::Snake);

    } else {
      const auto lastBlockPos = snakeBlocks.back().As1D(gridSize);
      const auto prevBlockPos = Coord::FromVector2(snakePosition).As1D(gridSize);
      // raylib::DrawText(TextFormat("%d", prevSnakeBlock.state), 0, 0, 40, raylib::Color::Pink());
      snakePosition += Coord::ToVector2(SnakeDirection) * deltatime * (snakeSpeed < 1 ? 1 : snakeSpeed) * blockSize * 0.1;

      // if we've moved to another block
      if (const auto snakeBlockPos = Coord::FromVector2(snakePosition).As1D(gridSize); snakeBlockPos != prevBlockPos) {
        // set the front position
        auto& frontSnakeBlock = blockGrid[snakeBlockPos];
        // if we run into an already-snake block, we lose
        if (static_cast<bool>(frontSnakeBlock.state & Block::State::Snake)) {
          GetScreenManager()->GoTo<LoseScreen>();
          return;
        }

        // unset and remove the last block of the snake, if we aren't revealing a new block
        if (static_cast<bool>(frontSnakeBlock.state & Block::State::Revealed)) {
          auto& lastSnakeBlock = blockGrid[lastBlockPos];
          lastSnakeBlock.state = static_cast<Block::State>(lastSnakeBlock.state & (~Block::State::Snake));
          snakeBlocks.pop_back();
        }
        snakeBlocks.emplace_front(Coord::AsCoord(snakeBlockPos, gridSize));
        frontSnakeBlock.state = static_cast<Block::State>(frontSnakeBlock.state | Block::State::Snake);
      }
    }

    raylib::DrawText(TextFormat("%d", snakeBlocks.size()), 0, 0, 40, raylib::Color::Pink());
    RevealFrom(Coord::FromVector2(snakePosition));

    // if we go out-of-bounds, we lose
    if (snakePosition.x > gridSize || snakePosition.y > gridSize || snakePosition.y < 0 || snakePosition.x < 0) {
      GetScreenManager()->GoTo<LoseScreen>();
      return;
    }

  } else {
    const auto pos = raylib::Mouse::GetPosition();
    if (raylib::Mouse::IsButtonReleased(MOUSE_BUTTON_LEFT)) {
      // check to see if we clicked in the grid
      if (pos.x >= offset.x && pos.y >= offset.y && pos.x < (offset.x + (gridDim)) && pos.y < offset.y + (gridDim)) {
        const Coord gridPos{static_cast<int>((pos.x - offset.x) / (blockSize)), static_cast<int>((pos.y - offset.y) / (blockSize))};
        if (!isGenerated) {
          GenerateGame(gridPos);
          blockTextSize = getTextSize(blockSize);
        }
        RevealFrom(gridPos);
      }
    } else if (raylib::Mouse::IsButtonReleased(MOUSE_BUTTON_RIGHT)) {
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
      snakePosition = raylib::Vector2{};
      SnakeDirection = {};
      numSafeBlocks = (gridSize * gridSize) - numBombs;
      isGenerated = false;
      const auto gridSizeSqr = static_cast<std::size_t>(gridSize) * static_cast<std::size_t>(gridSize);
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

  const auto gridSizeSqr = static_cast<std::size_t>(gridSize) * static_cast<std::size_t>(gridSize);

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
  isGenerated = true;
}
