#include "include/processor.h"

using std::string;
using std::vector;
const std::string kProcDirectory{"/proc/"};
const std::string kStatFilename{"/stat"};

// Returns the aggregate CPU utilization
vector<double> Processor::Utilization() {
    std::ifstream file(kProcDirectory + kStatFilename);
    std::string line;
    std::vector<double> cpuUtilization;
    if (file.is_open()) {
        int i = 0;
        while (std::getline(file, line)) {
            std::istringstream linestream(line);
            std::string cpu;
            linestream >> cpu;
            if (cpu.substr(0, 3) == "cpu") {
                long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
                if (linestream >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice) {
                    long totalTime = user + nice + system + idle + iowait + irq + softirq + steal + guest + guest_nice;
                    long workTime = idle + iowait;

                    if (prevWork.size() < 9) {
                        prevWork.push_back(workTime);
                        prevTotal.push_back(totalTime);
                        cpuUtilization.push_back(0);
                    } else {
                        long deltaWork = workTime - prevWork[i];
                        long deltaTotal = totalTime - prevTotal[i];
                        double cpuUtil = 1.0 - static_cast<double>(deltaWork) / deltaTotal;
                        cpuUtilization.push_back(cpuUtil * 100);
                        prevWork[i] = workTime;
                        prevTotal[i] = totalTime;
                        i++;
                    }
                }
            }
        }
    }
    return cpuUtilization;
}
