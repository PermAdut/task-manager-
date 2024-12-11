#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H
#include <algorithm>
#include <QString>
#include <QProcess>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include "include/process.h"
class ProcessManager {
public:
    ProcessManager();
    bool terminateProcess(int pid);
private:
    std::vector<Process> processes;
    void loadProcesses();
};

#endif // PROCESS_MANAGER_H
