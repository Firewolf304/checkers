//
// Created by firewolf on 03.01.25.
//

#ifndef CHECKERS_BOARD_HPP
#define CHECKERS_BOARD_HPP
#include "include.hpp"

namespace checkers::board {
    class board {
        ftxui::ScreenInteractive & screen;
        const int xSize;
        const int ySize;
        std::shared_ptr<ftxui::Node> place;

    public:
        board(ftxui::ScreenInteractive & screen, int x, int y)
                : screen(screen), xSize(x), ySize(y) {
            makePlace();
        }

        void makePlace() {
            std::vector<std::vector< std::shared_ptr< ftxui::Node > >> boardNode(ySize, std::vector<std::shared_ptr<ftxui::Node>>(xSize));

            for (int y = 0; y < this->ySize; y++) {
                for (int x = 0; x < this->xSize; x++) {
                    bool black = (x + y) % 2 == 0;
                    auto cell = ftxui::text("⛀") |
                                ftxui::bgcolor(black ? ftxui::Color::White : ftxui::Color::Black) |
                                ftxui::color(black ? ftxui::Color::Black : ftxui::Color::White) |
                                    ftxui::size(ftxui::WIDTH, ftxui::GREATER_THAN, 3) | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 2);
                    boardNode[y][x] = cell;

                }
            }
            this->place = ftxui::gridbox(boardNode);
        }

        auto getBoard() const {
            return place;
        }
    };

}

#endif //CHECKERS_BOARD_HPP
