#include "screens/game_screen.hpp"
#include "screens/lose_screen.hpp"
#include <memory>
#include <screen_manager.hpp>

ScreenManager::ScreenManager() 
  : screens({std::make_unique<WinScreen>(), std::make_unique<LoseScreen>(), std::make_unique<NewGameMenu>(), std::make_unique<GameScreen>()}){
  std::apply([&](auto& ...screen){
    (screen->Init(this),...);
  }, screens);
}
