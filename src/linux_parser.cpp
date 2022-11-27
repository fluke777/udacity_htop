#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"
#include "utils.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// In a file find a line that starts with a key and then parse a value at index.
// the index defaults to 0 which is the first value AFTER the key.
// Currently splits only on whitespace but we could lift it to parameter.
std::string LinuxParser::ParseValueFromFile(std::string path, std::string key, int index) {
  string line;
  string parsedKey;
  string value;
  std::ifstream filestream(path);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> parsedKey) {
        if (parsedKey == key) {
          std::getline(linestream, value);
          std::regex reg{"\\s+"};
          // Trim the string from the left
          // So the we can access first argument at 0 after split
          Utils::Ltrim(value);
          std::vector<std::string> parts = Utils::split(value, reg);
          return parts.at(index);
        }
      }
    }
  }
  return value;
}

// An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// Read a whole file as a string
// If the file does not exist return empty string
string LinuxParser::ReadFile(std::string path) {
  string line;
  std::ifstream stream(path);
  if (stream.is_open()) {
    std::getline(stream, line);
    return line;
  }
  return string();
}

// From a file where is only one line parse an nth value
string LinuxParser::ParseNthValue(std::string path, int index, std::string regex) {
  string line;
  std::ifstream stream(path);
  if (stream.is_open()) {
    std::getline(stream, line);
    Utils::Ltrim(line);
    std::regex reg{regex};
    std::vector<std::string> parts = Utils::split(line, reg);
    return parts.at(index);
  }
  // I guess this can happen if process directory is deleted
  // just before we look at stuff
  return string();
}

// An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// Parse the filesystem to get list of PIDs
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Compute system memory utilization
float LinuxParser::MemoryUtilization() {
    float memTotal = stof(ParseValueFromFile(kProcDirectory + kMeminfoFilename, "MemTotal:"));
    float memFree = stof(ParseValueFromFile(kProcDirectory + kMeminfoFilename, "MemFree:"));
    return (memTotal - memFree) / memTotal;
}

// Compute system uptime in seconds.
long LinuxParser::UpTime() {
  float upTotal;
  std::string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> upTotal;
  }
  return upTotal;
}

// Compute system cpu utilization.
// This is a vector of jiffies from stat file
// We still need to compute the utilization
vector<string> LinuxParser::CpuUtilization() {
  string line;
  string key;
  string value;
  std::vector<std::string> vect(1);
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == "cpu") {
          std::string rest;
          std::regex reg{"\\s+"};
          std::getline(linestream, rest);
          return Utils::split(rest, reg);
        }
      }
    }
  }
  return vect;

}

// Get the number of total processes
int LinuxParser::TotalProcesses() {
  return stoi(ParseValueFromFile(kProcDirectory + kStatFilename, "processes"));
}

// Get the number of running processes
int LinuxParser::RunningProcesses() {
  return stoi(ParseValueFromFile(kProcDirectory + kStatFilename, "procs_running"));
}

// I removed the parsing functions from here. Frankly it does not seem to
// advantageous at the moment to have them here and the process just call out.
// Isn't that sort of mitigating the encapsulation. Also wome of the functions
// (uptime) requires access to system.