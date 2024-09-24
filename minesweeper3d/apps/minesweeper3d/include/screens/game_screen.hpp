#pragma once

#include "Vector2.hpp"
#include <cstddef>
#include <cstdint>
#include <list>
#include <screen.hpp>

struct Coord {
  int x, y;
  [[nodiscard]] bool operator==(const Coord& other) const {
    return x == other.x && y == other.y;
  }
  [[nodiscard]] std::size_t As1D(int dimSize) const {
    return (x * dimSize) + y;
  }

  [[nodiscard]] static Coord AsCoord(int idx, int dimSize) {
    return Coord(idx/dimSize, idx % dimSize);
  }

};

struct CoordHash {
  std::size_t operator()(const Coord& coord) const {
    return std::hash<int>()(coord.x) ^ std::hash<int>()(coord.y);
  }
};


struct Block {

  enum class State : std::uint8_t {
    Hidden = 0,
    Revealed = 1,
    Snake = 2,
    Flagged = 3,
  };

  std::uint8_t value = 0;
  State state = State::Hidden;
  bool isBomb = false;
};

class GameScreen : public Screen {
public:
  GameScreen() = default;
  GameScreen(const GameScreen&) = default;
  GameScreen(GameScreen&&) = delete;
  GameScreen& operator=(const GameScreen&) = default;
  GameScreen& operator=(GameScreen&&) = delete;
  ~GameScreen() override = default;

  void Draw(raylib::Vector2 windowSize) override;

  void CameFrom(Screen* screen) override;

private:

  void GenerateGame();
  static void DrawBlock(Coord pos, Block block, int blockSize);
  void RevealFrom(Coord pos){};

  int gridSize = 3;
  int numBombs = 1;
  int snakeSpeed = 1;
  bool snakeMode = false;

  // We use a (doubly-linked-) list to represent blocks of our snake so we can add/remove on the front/end
  std::list<Coord> snakeBlocks;

  std::vector<Block> blockGrid;
  
};

