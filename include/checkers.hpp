//
// Created by firewolf on 03.01.25.
//

#ifndef CHECKERS_CHECKERS_HPP
#define CHECKERS_CHECKERS_HPP
#include "include.hpp"
#include "local.hpp"

namespace checkers {
    class checkers {
        ftxui::ScreenInteractive& screen_;
        board::board board;
        std::shared_ptr<ftxui::ComponentBase> render;
        int player = 1;
        std::chrono::time_point<std::chrono::system_clock, std::chrono::duration> start;
    public:
        checkers(ftxui::ScreenInteractive & tui) : screen_( tui) , board(tui, 10, 10) {
#ifdef ENABLE_BOOST_LOG
            boost::log::add_console_log(
                    std::cout,
                    boost::log::keywords::format = "[%TimeStamp%] [%Severity%] [%ThreadID%] => %Message%"
            );
            boost::log::add_common_attributes();
            BOOST_LOG_TRIVIAL(info) << "Logs enabled";
            BOOST_LOG_TRIVIAL(info) << "Creating board";
#endif
            auto start = std::chrono::high_resolution_clock::now();

            this->render = ftxui::Renderer([&] {

                //this->player = !(this->player);
                auto now = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> elapsed = now - start;
                return ftxui::window(ftxui::text("Checkers:") | ftxui::bold,ftxui::hbox({
                    // board place
                    ftxui::vbox({
                        ftxui::filler(),
                        this->board.getBoard(),
                        ftxui::filler()
                    }),
                    // menu
                    ftxui::vbox({
                        ftxui::text(std::format("Player: {}", player)) | ftxui::bold,
                        ftxui::separator(),
                        ftxui::vbox({
                            ftxui::text(std::format("Time: {} seconds", elapsed.count())),
                        })
                    })
                }));
            });
            render |= ftxui::CatchEvent([&](ftxui::Event event) {
                if (event == ftxui::Event::Character('q')) {
                    this->screen_.ExitLoopClosure()();
                    return true;
                }
                if (event == ftxui::Event::Character('p')) {
#ifdef ENABLE_BOOST_LOG
                    BOOST_LOG_TRIVIAL(info) << "Player changed";
#endif
                    player = !player;
                    return true;
                }
                return true;
            });

            while(true) {
                this->screen_.Loop(this->render);
            }

        }

        auto makeMenu() {
            auto now = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = now - start;
            return ftxui::hbox({
                ftxui::vbox({
                ftxui::filler(),
                this->board.getBoard(),
                ftxui::filler()
                }),
                ftxui::vbox({
                ftxui::text(std::format("Player: {}", player)) | ftxui::bold,
                ftxui::separator(),
                ftxui::vbox({
                ftxui::text(std::format("Time: {} seconds", elapsed.count())),
                })
                })
            });
        }

    };
}

#endif //CHECKERS_CHECKERS_HPP
