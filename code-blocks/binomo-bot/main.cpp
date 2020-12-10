#include <iostream>
#include <fstream>
#include <winsock2.h>
#include <windows.h>
#include <conio.h>

#include "bot\binomo-bot.hpp"

#define PROGRAM_VERSION "1.2"
#define PROGRAM_DATE "10.12.2020"

int main(int argc, char **argv) {
    std::cout << "binomo bot " << PROGRAM_VERSION << " ";
    std::cout << PROGRAM_DATE << std::endl << std::endl;

    binomo_bot::Settings settings(argc, argv);
    if(settings.is_error) {
        std::cout << "binomo bot: settings error!" << std::endl;
        std::system("pause");
        return EXIT_FAILURE;
    }

    std::cout << "binomo bot: initialization start" << std::endl << std::endl;

    binomo_bot::BinomoBot binomobot;

    /* инициализируем основные настройки */
    if(!binomobot.init_main(settings)) {
        std::cout << "binomo bot: BinomoBot--->init_main() error" << std::endl;
        std::system("pause");
        return EXIT_FAILURE;
    }

    if(!binomobot.init_candles_stream_mt4(settings)) {
        std::cout << "binomo bot: BinomoBot--->init_candles_stream_mt4() error" << std::endl;
        std::system("pause");
        return EXIT_FAILURE;
    }

    if(!binomobot.init_pipe_server(settings)) {
        std::cout << "binomo bot: BinomoBot--->init_pipe_server() error" << std::endl;
        std::system("pause");
        return EXIT_FAILURE;
    }

    std::cout << "binomo bot: initialization complete" << std::endl << std::endl;
    std::cout << "binomo bot: press space bar to exit the program" << std::endl << std::endl;

    //std::system("pause");
    /* обрабатываем нажатие клавиши для выхода из программы */
    while(true) {
        bool is_exit = false;
        if(kbhit()) {
            int c = getch();
            switch(c) {
                case VK_SPACE:
                is_exit = true;
                std::cout << "exit" << std::endl;
                break;
                default:
                if(settings.hotkeys.is_use) {
                    for(size_t i = 0; i < settings.hotkeys.hotkey.size(); ++i) {
                        std::string keys = settings.hotkeys.hotkey[i].key;
                        for(size_t k = 0; k < keys.size(); ++k) {
                            if(keys[k] == c) {
                                binomobot.open_bo(
                                    settings.hotkeys.hotkey[i].symbol,
                                    settings.hotkeys.hotkey[i].amount,
                                    settings.hotkeys.hotkey[i].direction,
                                    settings.hotkeys.hotkey[i].duration,
                                    settings);
                                std::cout
                                    << "binomo bot: press hot key: " << c
                                    << ", open bo-bet, symbol = "
                                    << settings.hotkeys.hotkey[i].symbol
                                    << std::endl;
                                break;
                            }
                        }
                    }
                } else {
                    std::cout << "binomo bot: press key: " << c << std::endl;
                }
                break;
            }
        } // if
        if(is_exit) break;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return 0;
}
