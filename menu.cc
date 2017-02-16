#include <iostream>
#include <limits>
#include <vector>

#include "menu.h"

static void show_help(const std::vector<menu_item> &items)
{
    for (const auto &i : items) {
        std::cout << " " << i.selection << "  - " << i.help << std::endl;
    }
    std::cout << " x  - Exit the menu" << std::endl
        << " ?  - Show this help " << std::endl;
}

void show_menu(const std::vector<menu_item> &items, bool auto_exit)
{
    bool done = false;

    while (!done) {
        char input;

        std::cout << "[";
        for (const auto &i : items) {
            std::cout << i.selection;
        }
        std::cout << "x?]: ";

        input = std::cin.get();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.clear();

        if (input == '?') {
            show_help(items);
        } else if (input == 'x') {
            done = true;
        } else {
            bool found = false;

            for (const auto &i : items) {
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
