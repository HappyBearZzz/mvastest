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
    std::cout << "Benchmark VAS create-delete" << std::flush;

    int i;
    typename std::chrono::high_resolution_clock::duration create = {}, del = {};
    typename std::chrono::high_resolution_clock::time_point start, stop;

    auto begin = std::chrono::high_resolution_clock::now();

    start = begin;
    for (i = 0; i < ITERATIONS; ++i) {
        auto vid = vas_create("test-create", 0600);
        if (vid < 0) {
            std::cout << "Failed to create VAS -- abort" << std::endl;
            break;
        }

        stop = std::chrono::high_resolution_clock::now();
        create += (stop - start);
        start = stop;

        if (vas_delete(vid) != 0) {
            std::cout << "Failed to delete VAS -- abort" << std::endl;
            break;
        }

        stop = std::chrono::high_resolution_clock::now();
        del += (stop - start);
        start = stop;
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::cout << " - DONE - " << i << " cycles " << std::endl
        << "total: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms"
        << " -- per cycle: " << std::chrono::duration_cast<std::chrono::nanoseconds>((end - begin)/ITERATIONS).count() << "ns" << std::endl
        << "create: " << std::chrono::duration_cast<std::chrono::milliseconds>(create).count() << "ms"
        << " -- per cycle: " << std::chrono::duration_cast<std::chrono::nanoseconds>(create/ITERATIONS).count() << "ns" << std::endl
        << "delete: " << std::chrono::duration_cast<std::chrono::milliseconds>(del).count() << "ms"
        << " -- per cycle: " << std::chrono::duration_cast<std::chrono::nanoseconds>(del/ITERATIONS).count() << "ns" << std::endl;
}

static void benchmark_attach()
{
    std::cout << "Benchmark VAS attach-detach" << std::flush;

    auto vid = vas_create("test-attach", 0600);
    if (vid < 0) {
        std::cout << "Failed to create VAS -- abort" << std::endl;
        return;
    }

    int i;
    typename std::chrono::high_resolution_clock::duration attach = {}, detach = {};
    typename std::chrono::high_resolution_clock::time_point start, stop;

    auto begin = std::chrono::high_resolution_clock::now();

    start = begin;
    for (i = 0; i < ITERATIONS; ++i) {
        if (vas_attach(0, vid, O_RDWR) != 0) {
            std::cout << "Failed to attach VAS -- abort" << std::endl;
            break;
        }

        stop = std::chrono::high_resolution_clock::now();
        attach += (stop - start);
        start = stop;

        if (vas_detach(0, vid) != 0) {
            std::cout << "Failed to detach VAS -- abort" << std::endl;
            break;
        }

        stop = std::chrono::high_resolution_clock::now();
        detach += (stop - start);
        start = stop;
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::cout << " - DONE - " << i << " cycles " << std::endl
        << "total: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms"
        << " -- per cycle: " << std::chrono::duration_cast<std::chrono::nanoseconds>((end - begin)/ITERATIONS).count() << "ns" << std::endl
        << "attach: " << std::chrono::duration_cast<std::chrono::milliseconds>(attach).count() << "ms"
        << " -- per cycle: " << std::chrono::duration_cast<std::chrono::nanoseconds>(attach/ITERATIONS).count() << "ns" << std::endl
        << "detach: " << std::chrono::duration_cast<std::chrono::milliseconds>(detach).count() << "ms"
        << " -- per cycle: " << std::chrono::duration_cast<std::chrono::nanoseconds>(detach/ITERATIONS).count() << "ns" << std::endl;

    if (vas_delete(vid) != 0) {
        std::cout << "Failed to delete VAS" << std::endl;
    }
}

static void benchmark_switch()
{
    std::cout << "Benchmark VAS switching" << std::flush;

    auto vid = vas_create("test-switch", 0600);
    if (vid < 0) {
        std::cout << "Failed to create VAS -- abort" << std::endl;
        return;
    }

    if (vas_attach(0, vid, O_RDWR) != 0) {
        std::cout << "Failed to attach VAS -- abort" << std::endl;
    } else {
        int i;
        typename std::chrono::high_resolution_clock::duration switch_in = {}, switch_out = {};
        typename std::chrono::high_resolution_clock::time_point start, stop;

        auto begin = std::chrono::high_resolution_clock::now();

        start = begin;
        for (i = 0; i < ITERATIONS; ++i) {
            if (vas_switch(vid) != 0) {
                std::cout << "Failed to switch to VAS -- abort" << std::endl;
                break;
            }

            stop = std::chrono::high_resolution_clock::now();
            switch_in += (stop - start);
            start = stop;

            if (vas_switch(0) != 0) {
                std::cout << "Failed to switch back to original AS -- abort" << std::endl;
                break;
            }

            stop = std::chrono::high_resolution_clock::now();
            switch_out += (stop - start);
            start = stop;
        }

        auto end = std::chrono::high_resolution_clock::now();

        std::cout << " - DONE - " << i << " cycles " << std::endl
            << "total: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms"
            << " -- per cycle: " << std::chrono::duration_cast<std::chrono::nanoseconds>((end - begin)/ITERATIONS).count() << "ns" << std::endl
            << "switch in: " << std::chrono::duration_cast<std::chrono::milliseconds>(switch_in).count() << "ms"
            << " -- per cycle: " << std::chrono::duration_cast<std::chrono::nanoseconds>(switch_in/ITERATIONS).count() << "ns" << std::endl
            << "switch out: " << std::chrono::duration_cast<std::chrono::milliseconds>(switch_out).count() << "ms"
            << " -- per cycle: " << std::chrono::duration_cast<std::chrono::nanoseconds>(switch_out/ITERATIONS).count() << "ns" << std::endl;

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
    std::cout << "Benchmark between VAS switching" << std::flush;

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
            typename std::chrono::high_resolution_clock::duration switch_vas1 = {}, switch_vas2 = {};
            typename std::chrono::high_resolution_clock::time_point start, stop;

            auto begin = std::chrono::high_resolution_clock::now();

            start = begin;
            for (i = 0; i < ITERATIONS; ++i) {
                if (vas_switch(vid2) != 0) {
                    std::cout << "Failed to switch to second VAS -- abort" << std::endl;
                    break;
                }

                stop = std::chrono::high_resolution_clock::now();
                switch_vas2 += (stop - start);
                start = stop;

                if (vas_switch(vid1) != 0) {
                    std::cout << "Failed to switch to first VAS -- abort" << std::endl;
                    break;
                }

                stop = std::chrono::high_resolution_clock::now();
                switch_vas1 += (stop - start);
                start = stop;
            }

            auto end = std::chrono::high_resolution_clock::now();

            std::cout << " - DONE - " << i << " cycles " << std::endl
                << "total: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms"
                << " -- per cycle: " << std::chrono::duration_cast<std::chrono::nanoseconds>((end - begin)/ITERATIONS).count() << "ns" << std::endl
                << "switch to VAS1: " << std::chrono::duration_cast<std::chrono::milliseconds>(switch_vas1).count() << "ms"
                << " -- per cycle: " << std::chrono::duration_cast<std::chrono::nanoseconds>(switch_vas1/ITERATIONS).count() << "ns" << std::endl
                << "switch to VAS2: " << std::chrono::duration_cast<std::chrono::milliseconds>(switch_vas2).count() << "ms"
                << " -- per cycle: " << std::chrono::duration_cast<std::chrono::nanoseconds>(switch_vas2/ITERATIONS).count() << "ns" << std::endl;

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
