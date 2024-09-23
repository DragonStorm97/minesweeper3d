#include <raygui.h>
#include <raylib-cpp.hpp>
#include <screens/main_menu.hpp>

void Main_Menu::Draw() {
  GuiButton(Rectangle{ 24, 24, 120, 30 }, "#191#Show Message");
}
