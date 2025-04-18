#include <libinput.h>
#include <string>
#include <sstream>
#include <libudev.h>
#include <iostream>
#include <fcntl.h> 
#include <unistd.h>
#include <vector>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <algorithm>

#include "attinv/attinvconf.hpp"
#include "attinv/utils.hpp"


static int open_restricted(const char *path, int flags, void *user_data) {
    int fd = open(path, flags);
    return fd < 0 ? -errno : fd;
}
 
static void close_restricted(int fd, void *user_data) {
    close(fd);
}
 
const static struct libinput_interface interface = {
    .open_restricted = open_restricted,
    .close_restricted = close_restricted,
};

void attinvLoop(AttinvConfig config) {
    using std::pair;
    using namespace std::literals;
    using std::vector;
    using namespace std::chrono;

    struct libinput *li;
    struct libinput_event *event;
    struct udev *udev = udev_new();

    li = libinput_udev_create_context(&interface, NULL, udev);
    libinput_udev_assign_seat(li, "seat0");

    vector<bool> keys(config.logHotkey.size(), 0);
    vector<timeInterval> intervalVec;
    time_point<system_clock> prevTime, currTime = system_clock::now();

    while (true) {
        libinput_dispatch(li);
        while ((event = libinput_get_event(li)) != NULL) {
            bool isKeyboard = libinput_event_get_type(event) == LIBINPUT_EVENT_KEYBOARD_KEY;
            if (isKeyboard) {
                struct libinput_event_keyboard *keyboardEvent = libinput_event_get_keyboard_event(event);
                uint32_t keycode = libinput_event_keyboard_get_key(keyboardEvent);
                for (int i = 0; i < config.logHotkey.size(); ++i) {
                    if (keycode == config.logHotkey[i])
                        keys[i] = !keys[i];
                }
            }

            if (std::all_of(keys.begin(), keys.end(), [&](int x) { return x == 1; })) {
                prevTime = currTime;
                currTime = system_clock::now();
                intervalVec.push_back(timeInterval{prevTime, currTime});
                auto diff = duration_cast<milliseconds>(currTime - prevTime);

                auto date = system_clock::to_time_t(prevTime);
                std::stringstream ss;
                ss << config.outputDir << "/" << std::put_time(std::localtime(&date), "%F") << ".log";

                writeToFile(ss.str(), intervalVec);
            }

            libinput_event_destroy(event);
        }
    }

    libinput_unref(li);

}
 
int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Expected one argument. Usage:" << std::endl;
        std::cerr << "attinv <CONFIG_FILE>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    AttinvConfig config(filename);
    attinvLoop(config);

    return 0;
}
