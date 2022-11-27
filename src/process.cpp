#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"
#include "utils.h"
#include "system.h"

using std::string;
using std::to_string;
using std::vector;

// Return pid specific base path
std::string Process::Path() {
    return LinuxParser::kProcDirectory + std::to_string(pid_);
}

// Return this process's ID
int Process::Pid() const {
    return pid_;
}

// Return this process's CPU utilization
float Process::CpuUtilization() {
    long hertz = sysconf(_SC_CLK_TCK);
    string path = Path() + LinuxParser::kStatFilename;
    float utime = stof(LinuxParser::ParseNthValue(path, 13));
    float stime = stof(LinuxParser::ParseNthValue(path, 14));
    float cutime = stof(LinuxParser::ParseNthValue(path, 15));
    float cstime = stof(LinuxParser::ParseNthValue(path, 16));
    long seconds = UpTime();
    float total_time = (utime + stime + cutime + cstime);
    if (seconds == 0) {
        return 0;
    } else {
        float cpu_usage = (total_time / hertz) / seconds;
        return cpu_usage;
    }
}

// Return the command that generated this process
string Process::Command() {
    std::string path = Path() + LinuxParser::kCmdlineFilename;
    auto result = LinuxParser::ReadFile(path);
    return result;
}

// TODO: Return this process's memory utilization
string Process::Ram() {
    string path = Path() + LinuxParser::kStatusFilename;
    string size = LinuxParser::ParseValueFromFile(path, "VmSize:");
    if (size == "") {
        return "0";
    } else {
        return std::to_string((stoi(size) / 1000));
    }
    
}

// Return the user (name) that generated this process
string Process::User() {
    int uid;
    // Parse UID from the process status
    string path = Path() + LinuxParser::kStatusFilename;
    string parsed_uid = LinuxParser::ParseValueFromFile(path, "Uid:");
    if (parsed_uid == "") {
        return "Unknown";
    } else {
        uid = stoi(parsed_uid);
    }

    // parse /etc/passwd and find a matching name (first column) for 
    // the uid which is on third column.
    string line;
    std::ifstream filestream(LinuxParser::kPasswordPath);
    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
          std::regex reg{":"};
          std::vector<std::string> parts = Utils::split(line, reg);
          if (stoi(parts.at(2)) == uid) {
              return parts.at(0);
          } else {
              return "Unknown";
          }
        }
    }

    // if user is not found let's return uid directly.
    // Probably would be good to also check an exception
    // the vector::at can throw if out of bounds. How likely is that?
    return std::to_string(uid);
}

// Return the age of this process (in seconds)
long int Process::UpTime() {
    long ticks = sysconf(_SC_CLK_TCK);
    std::string path = Path() + LinuxParser::kStatFilename;
    auto starttime = LinuxParser::ParseNthValue(path, 21);
    if (starttime == std::string()) {
        return 0L;
    } else {
        long result = stoi(starttime)/ticks;
        return system_->UpTime() - result;
    }
}

// Implement the operator.
// I removed it in favor of sorting based on lambda
// It does not feel great to have comparison operator on processes
// bool Process::operator<(Process& a) {
//     if ( stoi(Ram()) < stoi(a.Ram())) {
//         return false;
//     } else {
//         return true;
//     }
// }