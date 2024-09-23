#include <game.hpp>

constexpr static int Height = 600;
constexpr static int Width = 800;
constexpr static int TargetFps = 60;

int main()
{
  Game game(Width, Height, "Minesweeper3d", TargetFps);
  game.Start();
}


