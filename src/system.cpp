#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>
#include "include/linux_parser.h"
#include "include/process.h"
#include "include/processor.h"
#include "include/system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

/**
 * Return the system's CPU. 
 * Reserved for future additional features
 */
Processor& System::Cpu() { 
  return cpu_; 
}

// Returns a container composed of the system's processes
vector<Process>& System::Processes() { 
  vector<int> pids = LinuxParser::Pids();
  totalProcess = pids.size();
  processes_.clear();
  for(int pid: pids) {
    Process p;
    p.Pid(pid);
    p.User(pid);
    p.Command(pid);
    p.CpuUtilization(pid);
    p.Ram(pid);
    p.UpTime(pid);
    p.Threads(pid);
    processes_.push_back(p);
  }
  switch(sortingIndex){
    case 0:
      System::sortByPid(comp);
      break;
    case 1:
      System::sortByCommand(comp);
      break;
    case 2:
      System::sortByCpu(comp);
      break;
    case 3:
      System::sortByMem(comp);
      break;
    case 4:
      System::sortByTime(comp);
      break;
    case 5:
      System::sortByThreads(comp);
      break;
    case 6:
      System::sortByCommand(comp);
      break;
  default:
      break;
  }

  return processes_; 
}

std::vector<Process>& System::updateProcesses() {
    vector<int> currentPids = LinuxParser::Pids();
    set<int> existingPids;
    for (int pid : currentPids) {
        existingPids.insert(pid);
        bool found = false;
        for (auto& process : processes_) {
            if (process.Pid() == pid) {
                process.User(pid);
                process.Command(pid);
                process.CpuUtilization(pid);
                process.Ram(pid);
                process.UpTime(pid);
                process.Threads(pid);
                found = true;
                break;
            }
        }
        if (!found) {
            Process newProcess;
            newProcess.Pid(pid);
            newProcess.User(pid);
            newProcess.Command(pid);
            newProcess.CpuUtilization(pid);
            newProcess.Ram(pid);
            newProcess.UpTime(pid);
            newProcess.Threads(pid);
            processes_.push_back(newProcess);
        }
    }
    processes_.erase(
        std::remove_if(processes_.begin(), processes_.end(),
                       [&existingPids](Process& process) {
                           return existingPids.find(process.Pid()) == existingPids.end();
                       }),
        processes_.end());
    switch(sortingIndex){
      case 0:
        System::sortByPid(comp);
        break;
      case 1:
        System::sortByCommand(comp);
        break;
      case 2:
        System::sortByCpu(comp);
        break;
      case 3:
        System::sortByMem(comp);
        break;
      case 4:
        System::sortByTime(comp);
        break;
      case 5:
        System::sortByThreads(comp);
        break;
      case 6:
        System::sortByCommand(comp);
        break;
    default:
        break;
    }

    return processes_;
}

vector<Process>& System::getProcesses(){
    return processes_;
}

// Returns the system's kernel identifier (string)
std::string System::Kernel() { 
  return LinuxParser::Kernel();
}

// Returns the system's memory utilization
float System::MemoryUtilization() { 
  return LinuxParser::MemoryUtilization(); 
}

// Returns the operating system name
std::string System::OperatingSystem() { 
  return LinuxParser::OperatingSystem(); 
}

// Returns the number of processes actively running on the system
int System::RunningProcesses() { 
  return LinuxParser::RunningProcesses();
}

// Returns the total number of processes on the system
int System::TotalProcesses() { 
    vector<int> pids = LinuxParser::Pids();
    return pids.size();
}

// Returns the number of seconds since the system started running
long int System::UpTime() { 
  return LinuxParser::UpTime(); 
}

void System::swap(Process& a, Process& b) {
    Process temp = a;
    a = b;
    b = temp;
}

int System::partition(vector<Process>& processes, int low, int high, bool comp, const std::function<bool(Process&, Process&)>& comparator) {
    Process pivot = processes[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (comparator(processes[j], pivot) == comp) {
            i++;
            swap(processes[i], processes[j]);
        }
    }
    swap(processes[i + 1], processes[high]);
    return i + 1;
}

void System::quickSort(vector<Process>& processes, int low, int high, bool comp, const std::function<bool(Process&, Process&)>& comparator) {
    if (low < high) {
        int pi = partition(processes, low, high, comp, comparator);
        quickSort(processes, low, pi - 1, comp, comparator);
        quickSort(processes, pi + 1, high, comp, comparator);
    }
}

void System::sortByPid(bool comp) {
    sortingIndex = 0;
    System::quickSort(processes_, 0, processes_.size() - 1, comp, [](Process& a, Process& b) {
        return a.Pid() < b.Pid();
    });
}

void System::sortByCommand(bool comp) {
    sortingIndex = 6;
    System::quickSort(processes_, 0, processes_.size() - 1, comp, [](Process& a, Process& b) {
        return a.Command() < b.Command();
    });
}

void System::sortByCpu(bool comp) {
    sortingIndex = 2;
    System::quickSort(processes_, 0, processes_.size() - 1, comp, [](Process& a, Process& b) {
        return a.CpuUtilization() < b.CpuUtilization();
    });
}

void System::sortByMem(bool comp) {
    sortingIndex = 3;
    System::quickSort(processes_, 0, processes_.size() - 1, comp, [](Process& a, Process& b) {
        return static_cast<int>(std::stoi(a.Ram())) < static_cast<int>(std::stoi(b.Ram()));
    });
}

void System::sortByTime(bool comp) {
    sortingIndex = 4;
    System::quickSort(processes_, 0, processes_.size() - 1, comp, [](Process& a, Process& b) {
        return a.UpTime() < b.UpTime();
    });
}

void System::sortByUser(bool comp) {
    sortingIndex = 1;
    std::sort(processes_.begin(), processes_.end(), [comp](Process& a, Process& b) {
        return comp == 0 ? (a.User() < b.User()) : (a.User() > b.User());
    });
}

void System::sortByThreads(bool comp) {
    sortingIndex = 5;
    System::quickSort(processes_, 0, processes_.size() - 1, comp, [](Process& a, Process& b) {
        return a.Threads() < b.Threads();
    });
}
