#pragma once
#include <string>
#include <vector>
#include <chrono>

typedef std::pair<std::chrono::time_point<std::chrono::system_clock>, std::chrono::time_point<std::chrono::system_clock>> timeInterval;

void writeToFile(std::string filename, std::vector<timeInterval> intervalVec);
