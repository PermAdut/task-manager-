#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <string>
#include <vector>
#include "include/linux_parser.h"

class Processor {
 public:
  std::vector<double> Utilization();
  std::vector<long> prevWork = {};
  std::vector<long> prevTotal = {};
};

#endif
