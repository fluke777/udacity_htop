#include <iostream>
#include <string>

#include "processor.h"
#include "linux_parser.h"

// Compute the utilization of the system
float Processor::Utilization() {
    auto v = LinuxParser::CpuUtilization();
    float user = std::stof(v.at(1));
    float nice = std::stof(v.at(2));
    float system = std::stof(v.at(3));
    float idle = std::stof(v.at(4));
    float iowait = std::stof(v.at(5));
    float irq = std::stof(v.at(6));
    float softirq = std::stof(v.at(7));
    float steal = std::stof(v.at(8));

    float Idle = idle + iowait;
    float NonIdle = user + nice + system + irq + softirq + steal;
    
    float result = ((Idle - NonIdle) / Idle);
    return result;
}