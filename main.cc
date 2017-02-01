#include <iostream>

#include "menu.h"
#include "vastests.h"

int main()
{
    std::cout << "VAS testing program" << std::endl;

    show_menu({{'i', "Run interactive tests", interactive_tests},
               {'b', "Run benchmarks", benchmark_tests}});

    return 0;
}
