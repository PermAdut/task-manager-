#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <dirent.h>
#include <experimental/filesystem>
#include <fstream>
#include <iomanip>
#include <regex>
#include <string>
#include <unistd.h>
#include <vector>
namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

struct ProcessStats {
    unsigned long utime;
    unsigned long stime;
    unsigned long cutime;
    unsigned long cstime;
};

// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
std::string KeyValParser(std::string, std::string);
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();
ProcessStats getProcessStats(int pid);

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};

long getTotalCpuTime();
long Jiffies();
long ActiveJiffies();
long ActiveJiffies(int pid);
long IdleJiffies();

std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long int UpTime(int pid);
long int StartTime(int pid);
long int Threads(int pid);
extern std::vector<long> prevTotal;
extern std::vector<long> prevWork;
};

#endif
