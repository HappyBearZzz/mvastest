#include <mvas/vas.h>
#include <mvas/segment.h>

#include <fcntl.h>

#include <chrono>
#include <iostream>

#include "menu.h"
#include "vastests.h"


static constexpr int ITERATIONS = 10000;

static void benchmark_create()
{
    std::cout << "Benchmark VAS create-delete" << std::endl;

    int i;
    auto start = std::chrono::high_resolution_clock::now();

    for (i = 0; i < ITERATIONS; ++i) {
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

    std::cout << "DONE - " << i << " create-delete cycles " << std::endl
        << "Total: " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << "ms" << std::endl
        << "Per cycle: " << std::chrono::duration_cast<std::chrono::nanoseconds>((stop - start)/ITERATIONS).count() << "ns" << std::endl;
}

static void benchmark_attach()
{
    std::cout << "Benchmark VAS attach-detach" << std::endl;

    auto vid = vas_create("test-attach", 0600);
    if (vid < 0) {
        std::cout << "Failed to create VAS -- abort" << std::endl;
        return;
    }

    int i;
    auto start = std::chrono::high_resolution_clock::now();

    for (i = 0; i < ITERATIONS; ++i) {
        if (vas_attach(0, vid, O_RDWR) != 0) {
            std::cout << "Failed to attach VAS -- abort" << std::endl;
            break;
        }

        if (vas_detach(0, vid) != 0) {
            std::cout << "Failed to detach VAS -- abort" << std::endl;
            break;
        }
    }

    auto stop = std::chrono::high_resolution_clock::now();

    if (vas_delete(vid) != 0) {
        std::cout << "Failed to delete VAS" << std::endl;
    }

    std::cout << "DONE - " << i << " attach-detach cycles " << std::endl
        << "Total: " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << "ms" << std::endl
        << "Per cycle: " << std::chrono::duration_cast<std::chrono::nanoseconds>((stop - start)/ITERATIONS).count() << "ns" << std::endl;
}

void benchmark_tests()
{
    show_menu({{'c', "Benchmark create-delete of VAS", benchmark_create},
               {'a', "Benchmark attach-detach of VAS", benchmark_attach}});
}
