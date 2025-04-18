#pragma once
#include <string>
#include <vector>
#include <unordered_map>

struct AttinvConfig {
    std::string outputDir;
    std::string configFilename;
    std::vector<short> logHotkey = {29, 125, 25};
    AttinvConfig(std::string configFilename);
private:
    std::unordered_map<std::string, short> loadHotkeyMap();
    std::vector<short> parseHotkeyString(std::string hotkeyString);
};
