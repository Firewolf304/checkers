#include "./include/checkers.hpp"

int main() {
    auto screen = ftxui::ScreenInteractive::Fullscreen();

    checkers::checkers game(screen);
    game.Run();
    return 0;
}
