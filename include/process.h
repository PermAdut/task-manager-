#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class Process {
 public:
  // Accessors
  int Pid();
  std::string User();
  std::string Command();
  float CpuUtilization();
  std::string Ram();
  long int UpTime();
  long int Threads();
  // Mutators
  void Pid(int);
  void User(int);
  void Command(int);
  void CpuUtilization(int);
  void Ram(int);
  void UpTime(int);
  void Threads(int);

 private:
  int pid_{0};
  std::string user_{""};
  std::string command_{""};
  float cpu_utilization_{0.0};
  std::string ram_{""};
  long up_time_{0};
  long threads_;
  long totalTime_;
  long prevTotalTime_ = 0;
  long prevWtime_ = 0;
};

#endif
