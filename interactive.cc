#include <mvas/vas.h>
#include <mvas/segment.h>

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include <iostream>
#include <string>

#include "menu.h"
#include "vastests.h"


static void action_create_segment()
{
    std::string name;
    std::string start_string;
    unsigned long start;
    std::string end_string;
    unsigned long end;
    std::string mode_string;
    mode_t mode;
    segid_t sid;

    try {
        std::cout << "name:  ";
        std::cin >> name;
        std::cout << "start: ";
        std::cin >> start_string;
        start = stol(start_string, nullptr, 0);
        std::cout << "end:   ";
        std::cin >> end_string;
        end = stol(end_string, nullptr, 0);
        std::cout << "mode:  ";
        std::cin >> mode_string;
        mode = stoi(mode_string, nullptr, 0);
    } catch (std::invalid_argument&) {
        std::cout << "Invalid input" << std::endl;
        return;
    }

    if ((sid = segment_create(name.c_str(), start, end, mode)) < 0)
        std::cout << "Failed to create segment --> " << strerror(errno) << std::endl;
    else
        std::cout << "Successfully created a new segment [" << sid << "]" << std::endl;
}

static void action_create_vas()
{
    std::string name;
    std::string mode_string;
    mode_t mode;
    vasid_t vid;

    try {
        std::cout << "name: ";
        std::cin >> name;
        std::cout << "mode: ";
        std::cin >> mode_string;
        mode = std::stoi(mode_string, nullptr, 0);
    } catch (std::invalid_argument&) {
        std::cout << "Invalid input" << std::endl;
        return;
    }

    if ((vid = vas_create(name.c_str(), mode)) < 0)
        std::cout << "Failed to create VAS --> " << strerror(errno) << std::endl;
    else
        std::cout << "Successfully created a new VAS [" << vid << "]" << std::endl;
}

static void action_create()
{
    show_menu({{'s', "Create a new segment", action_create_segment},
               {'v', "Create a new VAS", action_create_vas}}, true);
}

static void action_delete_segment()
{
    segid_t sid;

    try {
        std::cout << "SID: ";
        std::cin >> sid;
    } catch (std::invalid_argument&) {
        std::cout << "Invalid input" << std::endl;
        return;
    }

    if (segment_delete(sid) != 0)
        std::cout << "Failed to delete segment --> " << strerror(errno) << std::endl;
    else
        std::cout << "Successfully deleted the segment" << std::endl;
}

static void action_delete_vas()
{
    vasid_t vid;

    try {
        std::cout << "VID: ";
        std::cin >> vid;
    } catch (std::invalid_argument&) {
        std::cout << "Invalid input" << std::endl;
        return;
    }

    if (vas_delete(vid) != 0)
        std::cout << "Failed to delete VAS --> " << strerror(errno) << std::endl;
    else
        std::cout << "Successfully deleted the VAS" << std::endl;
}

static void action_delete()
{
    show_menu({{'s', "Delete a segment", action_delete_segment},
               {'v', "Delete a VAS", action_delete_vas}}, true);
}

static void action_attach_segment()
{
    vasid_t vid;
    segid_t sid;
    std::string type_string;
    int type;

    try {
        std::cout << "VID: ";
        std::cin >> vid;
        std::cout << "SID: ";
        std::cin >> sid;

again:
        std::cout << "type (ro, wo, rw): ";
        std::cin >> type_string;
        if (type_string == "ro")
            type = O_RDONLY;
        else if (type_string == "wo")
            type = O_WRONLY;
        else if (type_string == "rw")
            type = O_RDWR;
        else {
            std::cout << "Huh?" << std::endl;
            goto again;
        }
    } catch (std::invalid_argument&) {
        std::cout << "Invalid input" << std::endl;
        return;
    }

    if (segment_attach(vid, sid, type) != 0)
        std::cout << "Failed to attach segment --> " << strerror(errno) << std::endl;
    else
        std::cout << "Successfully attached the segment" << std::endl;
}

static void action_attach_vas()
{
    pid_t pid;
    vasid_t vid;
    std::string type_string;
    int type;

    try {
        std::cout << "pid (0 == current): ";
        std::cin >> pid;
        std::cout << "VID: ";
        std::cin >> vid;

again:
        std::cout << "type (ro, wo, rw): ";
        std::cin >> type_string;
        if (type_string == "ro")
            type = O_RDONLY;
        else if (type_string == "wo")
            type = O_WRONLY;
        else if (type_string == "rw")
            type = O_RDWR;
        else {
            std::cout << "Huh?" << std::endl;
            goto again;
        }
    } catch (std::invalid_argument&) {
        std::cout << "Invalid input" << std::endl;
        return;
    }

    if (vas_attach(pid, vid, type) != 0)
        std::cout << "Failed to attach VAS --> " << strerror(errno) << std::endl;
    else
        std::cout << "Successfully attached the VAS" << std::endl;
}

static void action_attach()
{
    show_menu({{'s', "Attach a segment to a VAS", action_attach_segment},
               {'v', "Attach a VAS to a task", action_attach_vas}}, true);
}

static void action_detach_segment()
{
    vasid_t vid;
    segid_t sid;

    try {
        std::cout << "VID: ";
        std::cin >> vid;
        std::cout << "SID: ";
        std::cin >> sid;
    } catch (std::invalid_argument&) {
        std::cout << "Invalid input" << std::endl;
        return;
    }

    if (segment_detach(vid, sid) != 0)
        std::cout << "Failed to detach segment --> " << strerror(errno) << std::endl;
    else
        std::cout << "Successfully detached the segment" << std::endl;
}

static void action_detach_vas()
{
    pid_t pid;
    vasid_t vid;

    try {
        std::cout << "pid (0 == current): ";
        std::cin >> pid;
        std::cout << "VID: ";
        std::cin >> vid;
    } catch (std::invalid_argument&) {
        std::cout << "Invalid input" << std::endl;
        return;
    }

    if (vas_detach(pid, vid) != 0)
        std::cout << "Failed to detach VAS --> " << strerror(errno) << std::endl;
    else
        std::cout << "Successfully detached the VAS" << std::endl;
}

static void action_detach()
{
    show_menu({{'s', "Detach a segment from a VAS", action_detach_segment},
               {'v', "Detach a VAS from a task", action_detach_vas}}, true);
}

static void action_switch()
{
    vasid_t vid;

    try {
        std::cout << "VID (0 == original): ";
        std::cin >> vid;
    } catch (std::invalid_argument&) {
        std::cout << "Invalid input" << std::endl;
        return;
    }

    if (vas_switch(vid) != 0)
        std::cout << "Failed to switch VAS --> " << strerror(errno) << std::endl;
    else
        std::cout << "Successfully switched VAS" << std::endl;
}

void interactive_tests()
{
    show_menu({{'c', "Create a new VAS/segemnt", action_create},
               {'d', "Delete a VAS/segment", action_delete},
               {'a', "Attach a VAS/segment", action_attach},
               {'t', "Detach a VAS/segment", action_detach},
               {'s', "Switch to a VAS", action_switch}});
}
