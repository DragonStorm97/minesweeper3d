#include <screen.hpp>

class Main_Menu : public Screen {
public:
  Main_Menu() = default;
  Main_Menu(const Main_Menu&) = default;
  Main_Menu(Main_Menu&&) = delete;
  Main_Menu& operator=(const Main_Menu&) = default;
  Main_Menu& operator=(Main_Menu&&) = delete;
  ~Main_Menu() override = default;

  void Draw() override;

};
