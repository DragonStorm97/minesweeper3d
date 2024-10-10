#pragma once

#include "Vector2.hpp"
#include <cstddef>
#include <cstdint>
#include <list>
#include <screen.hpp>
#include <unordered_set>

struct Coord {
  int x, y;

  [[nodiscard]] bool operator==(const Coord& other) const
  {
    return x == other.x && y == other.y;
  }
  [[nodiscard]] std::size_t As1D(int dimSize) const
  {
    return (x * dimSize) + y;
  }

  [[nodiscard]] static Coord AsCoord(std::size_t idx, int dimSize)
  {
    return Coord(idx / dimSize, idx % dimSize);
  }

  static Coord FromVector2(const raylib::Vector2& vec) noexcept
  {
    return Coord{static_cast<int>(vec.x), static_cast<int>(vec.y)};
  }

  static raylib::Vector2 ToVector2(const Coord& coord) noexcept
  {
    return {static_cast<float>(coord.x), static_cast<float>(coord.y)};
  }
};

struct CoordHash {
  std::size_t operator()(const Coord& coord) const
  {
    return std::hash<int>()(coord.x) ^ std::hash<int>()(coord.y);
  }
};

struct Block {
  enum State : std::uint8_t {
    Hidden = 0b0000,
    Revealed = 0b0001,
    Snake = 0b0010,
    Flagged = 0b0100,
    Bomb = 0b1000
  };

  std::uint8_t value = 0;
  Block::State state = Block::State::Hidden;
};

class GameScreen : public Screen {
  public:
  GameScreen() = default;
  GameScreen(const GameScreen&) = default;
  GameScreen(GameScreen&&) = delete;
  GameScreen& operator=(const GameScreen&) = default;
  GameScreen& operator=(GameScreen&&) = delete;
  ~GameScreen() override = default;

  void Draw(float deltatime, raylib::Vector2 windowSize, bool wasResized) override;

  void CameFrom(Screen* screen) override;

  private:
  void GenerateGame(std::unordered_set<Coord, CoordHash>& safeBlocks);
  void DrawBlock(Coord pos, Block block, float blockSize) const;
  void RevealFrom(Coord pos);

  int gridSize = 3;
  int numBombs = 1;
  int snakeSpeed = 1;
  bool snakeMode = false;
  int numSafeBlocks = 9;

  // We use a (doubly-linked-) list to represent blocks of our snake so we can add/remove on the front/end
  std::list<Coord> snakeBlocks;

  std::vector<Block> blockGrid;

  bool isGenerated = false;
  int blockTextSize = 25;

  Coord SnakeDirection{0, 0};
  raylib::Vector2 snakePosition;
};
