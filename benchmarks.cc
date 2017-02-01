#include <mvas/vas.h>
#include <mvas/segment.h>

#include <chrono>
#include <iostream>

#include "menu.h"
#include "vastests.h"


static constexpr int ITERATIONS = 10000;

static void benchmark_create()
{
    std::cout << "Benchmark VAS create-delete" << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < ITERATIONS; ++i) {
        auto vid = vas_create("test-create", 0600);
        if (vid < 0) {
            std::cout << "Failed to create VAS -- abort" << std::endl;
            break;
        }

        if (vas_delete(vid) != 0) {
            std::cout << "Failed to delete VAS -- abort" << std::endl;
            break;
        }
    }

    auto stop = std::chrono::high_resolution_clock::now();

    std::cout << ITERATIONS << " create-delete cycles took "
        << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()
        << "ms" << std::endl;
}

void benchmark_tests()
{
    show_menu({{'c', "Benchmark create-delete of VAS", benchmark_create}});
}
