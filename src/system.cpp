#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric criteria "The student will be able to extract and display basic data about the system."

You need to properly format the uptime. Refer to the comments mentioned in format. cpp for formatting the uptime.*/

Processor& System::Cpu() {
    return cpu_;
    // Processor cpu()
}

// Return a container composed of the system's processes
vector<Process>& System::Processes() {
    auto pids = LinuxParser::Pids();
    processes_.clear();
    for (auto pid : pids) {
        const Process process{pid, this};
        processes_.push_back(process);
    }
    // I added sorting based on lambda.
    // Currently the lambda is hardcoded but I think it is
    // much more flexible than the overloaded operator.
    // If needed we probably could make the sorting more pluggable
    std::sort(processes_.begin(), processes_.end(), 
        [](Process & a, Process & b) -> bool { 
            return stoi(a.Ram()) > stoi(b.Ram()); 
    });
    return processes_;
}

// The following functions just delegate to the parser which
// feels a bit odd but I kept it here.



std::string System::Kernel() {
    return LinuxParser::Kernel();
}

float System::MemoryUtilization() {
    float result = LinuxParser::MemoryUtilization();
    return result;
}

std::string System::OperatingSystem() {
    return LinuxParser::OperatingSystem();
}

int System::RunningProcesses() { 
    return LinuxParser::RunningProcesses();
}

int System::TotalProcesses() { 
    return LinuxParser::TotalProcesses();
}

long int System::UpTime() {
    return LinuxParser::UpTime();
}
