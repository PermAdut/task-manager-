#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include "process.h"
#include "processor.h"

class System {
 public:
  Processor& Cpu();
  std::vector<Process>& Processes();
  std::vector<Process>& getProcesses();
  float MemoryUtilization();
  long UpTime();
  int TotalProcesses();
  int RunningProcesses();
  std::string Kernel();
  std::string OperatingSystem();
  std::vector<Process>& updateProcesses();
  void sortByPid(bool comp);
  void sortByCommand(bool comp);
  void sortByCpu(bool comp);
  void sortByMem(bool comp);
  void sortByTime(bool comp);
  void sortByUser(bool comp);
  void sortByThreads(bool comp);
  bool comp = false;
 private:
  void swap(Process& a, Process& b);
  void quickSort(std::vector<Process>& processes, int low, int high, bool comp, const std::function<bool(Process&, Process&)>& comparator);
  int partition(std::vector<Process>& processes, int low, int high, bool comp, const std::function<bool(Process&, Process&)>& comparator);
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
  int sortingIndex = -1;
  int totalProcess = 0;
};

#endif
