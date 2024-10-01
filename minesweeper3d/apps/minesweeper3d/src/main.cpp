#include <game.hpp>

constexpr static int Height = 600;
constexpr static int Width = 800;
constexpr static int TargetFps = 60;

int main()
{
  Game game(Width, Height, "Minesweep-Snake", TargetFps);
  game.Start();
}
