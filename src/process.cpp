#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "include/process.h"
#include "include/linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

//-----------------------------------------------------------------------------
// Accessors
//-----------------------------------------------------------------------------

// Return this process's ID
int Process::Pid() { 
  return pid_; 
}

// Return this process's CPU utilization
float Process::CpuUtilization() { 
  return cpu_utilization_;
}

// Return the command that generated this process
string Process::Command() { 
  return command_; 
}

// Return this process's memory utilization
string Process::Ram() { 
  return ram_;
}

// Return the user (name) that generated this process
string Process::User() { 
  return user_; 
}

// Return the age of this process (in seconds)
long Process::UpTime() { 
  return up_time_;
}

long Process::Threads(){
    return threads_;
}

//-----------------------------------------------------------------------------
// Mutators
//-----------------------------------------------------------------------------

void Process::Pid(int pid) {
  pid_ = pid;
}

void Process::User(int pid) {
  user_ = LinuxParser::User(pid);
}

void Process::Command(int pid) {
  command_ = LinuxParser::Command(pid);
}

void Process::CpuUtilization(int pid) {
    LinuxParser::ProcessStats stats = LinuxParser::getProcessStats(pid);
    long systemTime = LinuxParser::getTotalCpuTime();
    if(prevTotalTime_ == 0){
        prevTotalTime_ = systemTime;
        prevWtime_ = stats.utime + stats.stime;
        cpu_utilization_ = 0;
        return;
    }
    long deltaSystemTime = systemTime - prevTotalTime_;
    long deltaWorkTime = stats.utime + stats.stime - prevWtime_;
    if(deltaWorkTime > 0){
        cpu_utilization_ = static_cast<double>(deltaWorkTime) / deltaSystemTime * 8.0 * 100.0;
    } else {
        cpu_utilization_ = 0;
    }
    prevTotalTime_ = systemTime;
    prevWtime_ = stats.utime + stats.stime;
}

void Process::Ram(int pid) {
  int kbytes = std::atoi(LinuxParser::Ram(pid).c_str());
  int mbytes = kbytes / 1024;
  ram_ = std::to_string(mbytes);
}

void Process::UpTime(int pid){
  up_time_ = LinuxParser::UpTime(pid);
}

void Process::Threads(int pid){
    threads_ = LinuxParser::Threads(pid);
}
