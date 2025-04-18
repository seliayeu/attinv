#include <string>
#include <vector>
#include <fstream>
#include <iomanip>

#include "./utils.hpp"

void writeToFile(std::string filename, std::vector<timeInterval> intervalVec) {
    std::ofstream logFile(filename);

    for (timeInterval tI : intervalVec) {
        int mins = std::chrono::duration_cast<std::chrono::minutes>(tI.second - tI.first).count();
        int millis = std::chrono::duration_cast<std::chrono::milliseconds>(tI.second - tI.first).count();

        std::time_t tStart = std::chrono::system_clock::to_time_t(tI.first);
        std::time_t tEnd = std::chrono::system_clock::to_time_t(tI.second);

        logFile << std::setfill('0') << std::setw(2) << mins << "." << std::setfill('0') << std::setw(3)  << (int) (millis % 60000 / 60000.0 * 1000);
        logFile << ": " << std::put_time(std::localtime(&tStart), "%T") << " to " << std::put_time(std::localtime(&tEnd), "%T") << "\n";
    }
    logFile.close();
}


