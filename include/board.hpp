//
// Created by firewolf on 03.01.25.
//

#ifndef CHECKERS_BOARD_HPP
#define CHECKERS_BOARD_HPP
#include <utility>

#include "include.hpp"



namespace checkers::board {

    template<int width = 3, int height = 2, std::array<ftxui::Color::Palette16, 3> colors = {ftxui::Color::Black, ftxui::Color::White}>
    class figure {
        std::string figureCharacter;
        int x = -1;
        int y = -1;
        int player;
        std::shared_ptr<ftxui::Node> component;
    public:
        figure(int x, int y, int player, const std::string & figureCharacter = "⛀") : x(x), y(y), player(player), figureCharacter(figureCharacter){
            this->component = ftxui::text(figureCharacter) | ftxui::hcenter | ftxui::vcenter
                              | ftxui::bgcolor(getBgColor(x,y))
                              | ftxui::color(getColor(x,y))
                              | ftxui::size(ftxui::WIDTH, ftxui::GREATER_THAN, width)
                              | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, height) ;
        }

        ftxui::Color::Palette16 getColor(int xPos, int yPos) {
            return colors[(x + y) % 2 == 0];
        }
        ftxui::Color::Palette16 getBgColor(int xPos, int yPos) {
            return colors[(x + y) % 2 != 0];
        }
        auto getComponent() {
            return component;
        }
    private:
        void onClick() {
#ifdef ENABLE_BOOST_LOG
            BOOST_LOG_TRIVIAL(debug) << std::format("Checked [{}, {}]", this->x, this->y);
#endif
        }
    };

    template<typename Screen, int WIDTHCELL = 3, int HEIGHTCELL = 2>
    class board {
        Screen & screen;
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
                    // === logical arrangement of figures ===

                    /*bool black = (x + y) % 2 == 0;
                    auto cell = ftxui::text("⛀") |
                                ftxui::bgcolor(black ? ftxui::Color::White : ftxui::Color::Black) |
                                ftxui::color(black ? ftxui::Color::Black : ftxui::Color::White) |
                                    ftxui::size(ftxui::WIDTH, ftxui::GREATER_THAN, WIDTHCELL) | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, HEIGHTCELL);*/
                    auto cell = figure{x,y,1, "@"};
                    boardNode[y][x] = cell.getComponent();

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
