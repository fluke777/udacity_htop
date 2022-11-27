#include <string>
#include <iomanip>
#include <sstream>
#include <vector>
#include <regex>
#include "utils.h"


void Utils::Ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

std::vector<std::string> Utils::split(std::string text, std::regex split_regex) {
    std::vector<std::string> parts(std::sregex_token_iterator(text.begin(), text.end(), split_regex, -1), std::sregex_token_iterator());
    return parts;
}