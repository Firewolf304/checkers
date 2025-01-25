#include "./include/checkers.hpp"

int main() {
    auto screen = ftxui::ScreenInteractive::Fullscreen();

    checkers::checkers<ftxui::ScreenInteractive> game(screen);
    game.Run();
    return 0;
}
