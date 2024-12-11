#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTimer>
#include <QVector>
#include "ProcessManager.h"
#include "include/process.h"
#include "format.h"
#include <unistd.h>
#include "include/system.h"
#include <signal.h>
class TaskManager : public QMainWindow {
    Q_OBJECT

public:
    TaskManager();

private slots:
    void loadProcessData();
    void terminateSelectedProcess();
    void stopSelectedProcess();
    void hangupSelectedProcess();
    bool terminateProcess(int pid);
    bool sendSignal(int pid, int signal);
    void contextMenuEvent(QContextMenuEvent *event);
    void filterProcesses(const QString &searchText);
    void sortTable(int column);
private:
    System* system = new System();
    QTableWidget *processTable;
    std::vector<Process> processes;
    QLineEdit *searchLineEdit;
    bool ascendingSort[7];
    int columnId = -1;
};

#endif // TASKMANAGER_H
