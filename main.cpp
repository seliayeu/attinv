#include <libinput.h>
#include <string>
#include <libudev.h>
#include <iostream>
#include <fstream>
#include <fcntl.h> 
#include <unistd.h>
#include <vector>
#include <chrono>
#include <utility>
#include <ctime>
#include <iomanip>

typedef std::pair<std::chrono::time_point<std::chrono::system_clock>, std::chrono::time_point<std::chrono::system_clock>> timeInterval;

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

void writeToFile(std::string filename, std::vector<timeInterval> intervalVec) {
    std::ofstream logFile(filename);

    for (timeInterval tI : intervalVec) {
        int mins = std::chrono::duration_cast<std::chrono::minutes>(tI.second - tI.first).count();
        int millis = std::chrono::duration_cast<std::chrono::milliseconds>(tI.second - tI.first).count();

        std::time_t tStart = std::chrono::system_clock::to_time_t(tI.first);
        std::time_t tEnd = std::chrono::system_clock::to_time_t(tI.second);
        logFile << mins << "." << (int) ((millis % 60000) / 60000.0) * 1000;
        logFile << ": " << std::put_time(std::localtime(&tStart), "%F %T") << " to " << std::put_time(std::localtime(&tEnd), "%F %T") << "\n";
    }
    logFile.close();
}

 
int main(void) {
    using std::pair;
    using namespace std::literals;
    using std::vector;
    using namespace std::chrono;


    struct libinput *li;
    struct libinput_event *event;
    struct udev *udev = udev_new();

    li = libinput_udev_create_context(&interface, NULL, udev);
        libinput_udev_assign_seat(li, "seat0");

    bool keys[3] = {0, 0, 0};
    vector<timeInterval> intervalVec;
    time_point<system_clock> prevTime, currTime = system_clock::now();

    while (true) {
        libinput_dispatch(li);
        while ((event = libinput_get_event(li)) != NULL) {
            bool isKeyboard = libinput_event_get_type(event) == LIBINPUT_EVENT_KEYBOARD_KEY;
            if (isKeyboard) {
                struct libinput_event_keyboard *keyboardEvent = libinput_event_get_keyboard_event(event);
                uint32_t keycode = libinput_event_keyboard_get_key(keyboardEvent);
                if (keycode == 29) // Ctrl
                    keys[0] = !keys[0];
                else if (keycode == 125) // Super
                    keys[1] = !keys[1];
                else if (keycode == 25) // P
                    keys[2] = !keys[2];
            }

            if (keys[0] && keys[1] && keys[2]) {
                prevTime = currTime;
                currTime = system_clock::now();
                intervalVec.push_back(timeInterval{prevTime, currTime});
                auto diff = duration_cast<milliseconds>(currTime - prevTime);
                std::cout << (diff.count() / 1000) << "." << (diff.count() % 1000) << std::endl;
            }

            libinput_event_destroy(event);
        }
    }

    libinput_unref(li);

    return 0;
}
