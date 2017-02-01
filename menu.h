#ifndef __MENU_H__
#define __MENU_H__

#include <vector>

struct menu_item {
    char selection;
    const char *help;
    void (*func)();
};

extern void show_menu(const std::vector<menu_item> &items,
                      bool auto_exit=false);

#endif
