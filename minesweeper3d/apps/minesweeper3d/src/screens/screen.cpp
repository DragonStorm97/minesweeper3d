#include "screens/game_screen.hpp"
#include "screens/main_menu.hpp"
#include <memory>
#include <screen_manager.hpp>

ScreenManager::ScreenManager() 
  : screens({std::make_unique<MainMenu>(), std::make_unique<NewGameMenu>(), std::make_unique<GameScreen>()}){
  std::apply([&](auto& ...screen){
    (screen->Init(this),...);
  }, screens);
}
