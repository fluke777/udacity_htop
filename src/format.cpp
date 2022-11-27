#include <string>
#include <iomanip>
#include <sstream>
#include <vector>
#include <regex>
#include "format.h"

using std::string;


/*
Function returns a fixed width number padded with zeroes.
This is used to keep conssitent time formatting.
*/
string paddedInt(int value, int width) {
    std::stringstream ss;
    ss << std::setw(width) << std::setfill('0') << value;
    return ss.str();
}

// Format the elapsed time as HH:MM:SS
string Format::ElapsedTime(long elapsedSeconds) {
    int seconds = elapsedSeconds % 60;
    elapsedSeconds = (int) elapsedSeconds / 60;
    int minutes = elapsedSeconds % 60;
    elapsedSeconds = (int) elapsedSeconds / 60;
    int hours = elapsedSeconds;
    std::stringstream ss;
    ss << hours << ":" << paddedInt(minutes, 2) << ":" << paddedInt(seconds, 2);
    return ss.str();
}