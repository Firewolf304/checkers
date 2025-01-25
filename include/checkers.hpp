//
// Created by firewolf on 03.01.25.
//

#ifndef CHECKERS_CHECKERS_HPP
#define CHECKERS_CHECKERS_HPP
#include "include.hpp"
#include "local.hpp"

namespace checkers {
    using namespace std::chrono_literals;

    template<typename Screen, int minScreenX = 49, int minScreenY = 12, bool ignoreMinScreen = false>
    class checkers {
        Screen& screen_;
        board::board<Screen> board;
        std::shared_ptr<ftxui::ComponentBase> render;
        int player = 1;
        std::chrono::time_point<std::chrono::system_clock, std::chrono::duration<double>> start;
        std::shared_ptr<ftxui::Loop> loop;
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
            this->start = std::chrono::high_resolution_clock::now();
            this->render = ftxui::Renderer([&] {

                //this->player = !(this->player);
                //auto now = std::chrono::high_resolution_clock::now();
                //std::chrono::duration<double> elapsed = now - this->start;
                if((this->screen_.dimx() < minScreenX || this->screen_.dimy() < minScreenY) && !ignoreMinScreen) {
                    return ftxui::window(ftxui::text("WARNING") | color(ftxui::Color::Red),ftxui::vbox({
                        ftxui::filler(),
                        ftxui::text("⚠️ Terminal is too small!") | ftxui::center | ftxui::bold | color(ftxui::Color::Red),
                        ftxui::text("Please resize your terminal to at least 49x12.") | ftxui::center,
                        ftxui::filler(),
                    }) | ftxui::border) | size(ftxui::WIDTH, ftxui::GREATER_THAN, 10) | size(ftxui::HEIGHT, ftxui::GREATER_THAN, 10);
                } else {
                    return ftxui::window(ftxui::text("Checkers") | ftxui::bold, ftxui::hbox({
                            ftxui::hbox({
                                // board place
                                ftxui::vbox({
                                    ftxui::filler(),
                                    this->board.getBoard(),
                                    ftxui::filler()
                                }),

                                // menu
                                this->makeMenu() | ftxui::border | ftxui::size(ftxui::WIDTH,ftxui::EQUAL,20),
                                ftxui::filler(),
                                ftxui::vbox(ftxui::Button("Exit", [](){std::exit(0);}))
                            }) | ftxui::flex }) | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, 20)
                        ) | size(ftxui::WIDTH, ftxui::GREATER_THAN, 49) | size(ftxui::HEIGHT, ftxui::GREATER_THAN, 12);
                }
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
                return false;
            });


        }
        void Run() {
#ifdef ENABLE_BOOST_LOG
            BOOST_LOG_TRIVIAL(info) << "Starting (za)loopper";
#endif
            // по-хорошему обработку loop в отдельный поток без задержки, а кастомный ивент в loop с задержкой также в отдельный поток (нужно тредпул, либо обоих засинхронить)
            this->loop = std::make_shared<ftxui::Loop>(&this->screen_, this->render);
            auto SaveRender = this->render;
            while (!loop->HasQuitted()) {
                loop->RunOnce();

                //std::this_thread::sleep_for(std::chrono::milliseconds(10s));
                this->screen_.PostEvent(ftxui::Event::Custom);
                std::this_thread::sleep_for(std::chrono::milliseconds(100ms));
            }
            this->screen_.ExitLoopClosure();
        }
        [[nodiscard]] ftxui::Element makeMenu() const {
            auto now = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = now - start;
            double total = elapsed.count();
            double days = static_cast<double>(total / 86400);
            double hours = static_cast<double>(total / 3600);
            double minutes = static_cast<double>(total / 60);
            return ftxui::vbox({
                ftxui::text(std::format("Player: {}", player)) | ftxui::bold,
                ftxui::text(std::format("Screen: {}x{}", this->screen_.dimx(), this->screen_.dimy())),
                ftxui::separator(),
                //ftxui::vbox({ ftxui::text(std::format("Time: {} seconds", std::round(elapsed.count() * 100)/100)) }),
                ftxui::vbox({ ftxui::text(std::format("Time: {}{}{}{}",
                                                      days >= 1 ? std::format("{}d ", std::round(days * 100)/100) : "",
                                                      hours >= 1 && days < 1 ? std::format("{}h ", std::round(hours * 100)/100) : "",
                                                      minutes >= 1 && days < 1 && hours < 1 ? std::format("{}m ", std::round(minutes * 100)/100) : "",
                                                      (days < 1 && hours < 1 && minutes < 1) ? std::format("{}s", static_cast<int>(total)) : ""
                ))}),
                ftxui::filler()
            });
        }

    };
}

#endif //CHECKERS_CHECKERS_HPP
