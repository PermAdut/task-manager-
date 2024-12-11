#include "ProcessManager.h"
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QRegularExpression>
#include <QTextStream>
#include <QProcess>
#include <iostream>
#include <QMessageBox>
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

#define PROC_DIRECTORY "/proc/"

ProcessManager::ProcessManager() {
    loadProcesses();
}



void ProcessManager::loadProcesses() {
    processes.clear();

}



