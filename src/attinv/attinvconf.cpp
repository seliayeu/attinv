#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include "./attinvconf.hpp"

AttinvConfig::AttinvConfig(std::string configFilename) : configFilename(configFilename) {
    std::ifstream configFile(configFilename);
    std::string optionName;
    char tmp;
    
    while (configFile >> optionName) {
        configFile >> tmp;
        if (tmp != '=') {
            // TODO: look into C++ error throwing (is it cursed? is it standard?)
            std::cerr << "Incorrectly formatted config file." << std::endl;
            exit(1);
        }
        if (optionName == "log_dir")
            configFile >> outputDir;
        else if (optionName == "log_hotkey") {
            std::string hotkeyString;
            configFile >> hotkeyString;
            logHotkey = parseHotkeyString(hotkeyString);
        } else {
            std::cerr << "Invalid option name." << std::endl;
            exit(1);
        }
    }
}

std::unordered_map<std::string, short> AttinvConfig::loadHotkeyMap() {
    // assumes existence of keycodes.txt
    std::ifstream keycodeFile("keycodes.txt");
    std::unordered_map<std::string, short> hotkeyMap;
    std::string hotkey;
    short value;

    while (keycodeFile >> hotkey >> value)
        hotkeyMap[hotkey] = value;

    return hotkeyMap;
}

std::vector<short> AttinvConfig::parseHotkeyString(std::string hotkeyString) {
    // assume string has form "?-?-...-?"
    std::vector<short> keys;
    auto hotkeyMap = loadHotkeyMap();
    
    while (hotkeyString.find("-") != std::string::npos) {
        std::string hotkey = hotkeyString.substr(0, hotkeyString.find("-"));
        hotkeyString.erase(0, hotkeyString.find("-") + 1);
        keys.push_back(hotkeyMap[hotkey]);
    }
    keys.push_back(hotkeyMap[hotkeyString]);

    return keys;
}
