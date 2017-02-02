#include <mvas/vas.h>
#include <mvas/segment.h>

#include <fcntl.h>

#include <chrono>
#include <iostream>

#include "menu.h"
#include "vastests.h"


static constexpr int ITERATIONS = 100000;

static void benchmark_create()
{
    std::cout << "Benchmark VAS create-delete";

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

    std::cout << " - DONE - " << i << " cycles " << std::endl
        << "total: " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << "ms"
        << " -- per cycle: " << std::chrono::duration_cast<std::chrono::nanoseconds>((stop - start)/ITERATIONS).count() << "ns" << std::endl;
}

static void benchmark_attach()
{
    std::cout << "Benchmark VAS attach-detach";

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

    std::cout << " - DONE - " << i << " cycles " << std::endl
        << "total: " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << "ms"
        << " -- per cycle: " << std::chrono::duration_cast<std::chrono::nanoseconds>((stop - start)/ITERATIONS).count() << "ns" << std::endl;

    if (vas_delete(vid) != 0) {
        std::cout << "Failed to delete VAS" << std::endl;
    }
}

static void benchmark_switch()
{
    std::cout << "Benchmark VAS switching";

    auto vid = vas_create("test-switch", 0600);
    if (vid < 0) {
        std::cout << "Failed to create VAS -- abort" << std::endl;
        return;
    }

    if (vas_attach(0, vid, O_RDWR) != 0) {
        std::cout << "Failed to attach VAS -- abort" << std::endl;
    } else {
        int i;
        auto start = std::chrono::high_resolution_clock::now();

        for (i = 0; i < ITERATIONS; ++i) {
            if (vas_switch(vid) != 0) {
                std::cout << "Failed to switch to VAS -- abort" << std::endl;
                break;
            }

            if (vas_switch(0) != 0) {
                std::cout << "Failed to switch back to original AS -- abort" << std::endl;
                break;
            }
        }

        auto stop = std::chrono::high_resolution_clock::now();

        std::cout << " - DONE - " << i << " cycles " << std::endl
            << "total: " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << "ms"
            << " -- per cycle: " << std::chrono::duration_cast<std::chrono::nanoseconds>((stop - start)/ITERATIONS).count() << "ns" << std::endl;

        if (vas_detach(0, vid) != 0) {
            std::cout << "Failed to detach VAS" << std::endl;
        }
    }

    if (vas_delete(vid) != 0) {
        std::cout << "Failed to delete VAS" << std::endl;
    }
}

static void benchmark_switch_between()
{
    std::cout << "Benchmark between VAS switching";

    auto vid1 = vas_create("test-switch1", 0600);
    auto vid2 = vas_create("test-switch2", 0600);
    if (vid1 < 0 || vid2 < 0) {
        std::cout << "Failed to create VAS -- abort" << std::endl;
        return;
    }

    if ((vas_attach(0, vid1, O_RDWR) != 0) || (vas_attach(0, vid2, O_RDWR) != 0)){
        std::cout << "Failed to attach VAS -- abort" << std::endl;
    } else {
        if (vas_switch(vid1) != 0) {
            std::cout << "Failed to perform initial switch -- abort" << std::endl;
        } else {
            int i;
            auto start = std::chrono::high_resolution_clock::now();

            for (i = 0; i < ITERATIONS; ++i) {
                if (vas_switch(vid2) != 0) {
                    std::cout << "Failed to switch to second VAS -- abort" << std::endl;
                    break;
                }

                if (vas_switch(vid1) != 0) {
                    std::cout << "Failed to switch to first VAS -- abort" << std::endl;
                    break;
                }
            }

            auto stop = std::chrono::high_resolution_clock::now();

            std::cout << " - DONE - " << i << " cycles " << std::endl
                << "total: " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << "ms"
                << " -- per cycle: " << std::chrono::duration_cast<std::chrono::nanoseconds>((stop - start)/ITERATIONS).count() << "ns" << std::endl;

            if (vas_switch(0) != 0) {
                std::cout << "Failed to switch back to original AS" << std::endl;
            }
        }

        if ((vas_detach(0, vid2) != 0) || (vas_detach(0, vid1) != 0)) {
            std::cout << "Failed to detach VAS" << std::endl;
        }
    }

    if ((vas_delete(vid2) != 0) || (vas_delete(vid1) != 0)) {
        std::cout << "Failed to delete VAS" << std::endl;
    }
}

void benchmark_tests()
{
    show_menu({{'c', "Benchmark create-delete of VAS", benchmark_create},
               {'a', "Benchmark attach-detach of VAS", benchmark_attach},
               {'s', "Benchmark switching of VAS", benchmark_switch},
               {'b', "Benchmark switching between VAS", benchmark_switch_between}});
}
