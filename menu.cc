#include <iostream>
#include <vector>

#include "menu.h"

void show_menu(const std::vector<menu_item> &items, bool auto_exit)
{
    bool done = false;

    while (!done) {
        char input;

        std::cout << "[";
        for (const auto& i : items) {
            std::cout << i.selection;
        }
        std::cout << "x?]: ";

        std::cin >> input;

        if (input == '?') {
            std::cout << std::endl;
            for (const auto&i : items) {
                std::cout << " " << i.selection << "  - " << i.help << std::endl;
            }
            std::cout << " x  - Exit the menu" << std::endl
                << " ?  - Show this help " << std::endl;
        } else if (input == 'x') {
            done = true;
        } else {
            bool found = false;

            for (const auto&i : items) {
                if (i.selection == input) {
                    i.func();
                    if (auto_exit)
                        done = true;

                    found = true;
                    break;
                }
            }

            if (!found)
                std::cout << "Huh? " << input << std::endl;
        }
    }
}
