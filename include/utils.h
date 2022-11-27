#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <iomanip>
#include <sstream>
#include <vector>
#include <regex>

namespace Utils {
    void Ltrim(std::string &s);
    std::vector<std::string> split(std::string text, std::regex split_regex);
}
#endif