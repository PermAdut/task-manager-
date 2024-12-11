#include "include/system.h"
#include "TaskManager.h"
#include "SystemStatsWidget.h"
#include "FileSystemWidget.h"
#include <QTabWidget>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QPushButton>
#include <QMessageBox>
#include <QMenu>
#include <QContextMenuEvent>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QTimer>

TaskManager::TaskManager() {
    QTabWidget *tabWidget = new QTabWidget(this);

    QWidget *processesTab = new QWidget();
    QVBoxLayout *processesLayout = new QVBoxLayout();
    system->Processes();
    processTable = new QTableWidget();
    processTable->setColumnCount(7);
    processTable->setHorizontalHeaderLabels({"PID", "Пользователь", "CPU(%)", "RAM(MB)", "Время","Потоки","Команда"});
    processTable->horizontalHeader()->setStretchLastSection(true);
    for (int i = 0; i < processTable->columnCount(); ++i) {
        processTable->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    }
    connect(processTable->horizontalHeader(), &QHeaderView::sectionClicked, this, &TaskManager::sortTable);
    searchLineEdit = new QLineEdit();
    searchLineEdit->setPlaceholderText("Поиск по команде...");


    connect(searchLineEdit, &QLineEdit::textChanged, [this](const QString &text) {
        filterProcesses(text);
    });

    for (int i = 0; i < 7; ++i) {
        ascendingSort[i] = false;
    }
    system->Processes();
    QHBoxLayout *searchLayout = new QHBoxLayout();
    searchLayout->addWidget(searchLineEdit);
    processesLayout->addLayout(searchLayout);
    processesLayout->addWidget(processTable);
    processesTab->setLayout(processesLayout);
    tabWidget->addTab(processesTab, "Все процессы");
    SystemStatsWidget *systemStatsWidget = new SystemStatsWidget();
    tabWidget->addTab(systemStatsWidget, "Ресурсы");
    FileSystemWidget *fileSystemWidget = new FileSystemWidget();
    tabWidget->addTab(fileSystemWidget, "Информация о системе");
    setCentralWidget(tabWidget);
    QMenuBar *menuBar = new QMenuBar(this);
    setMenuBar(menuBar);
    setWindowTitle("Диспетчер задач");
    resize(800, 600);
    loadProcessData();
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &TaskManager::loadProcessData);
    timer->start(1000);
}

void TaskManager::loadProcessData() {
    QString currentSearchText = searchLineEdit->text();
    processes = system->updateProcesses();
    processTable->setRowCount(0);
    processTable->setRowCount(processes.size());
    switch (columnId) {
        case 0:
            system->sortByPid(ascendingSort[columnId]);
            break;
        case 1:
            system->sortByCommand(ascendingSort[columnId]);
            break;
        case 2:
            system->sortByCpu(ascendingSort[columnId]);
            break;
        case 3:
            system->sortByMem(ascendingSort[columnId]);
            break;
        case 4:
            system->sortByTime(ascendingSort[columnId]);
            break;
        case 5:
            system->sortByThreads(ascendingSort[columnId]);
            break;
        case 6:
            system->sortByCommand(ascendingSort[columnId]);
            break;
        default:
            break;
    }
    for (int i = 0; i < processes.size(); ++i) {
        processTable->setItem(i, 0, new QTableWidgetItem(QString::number(processes[i].Pid())));
        processTable->setItem(i, 1, new QTableWidgetItem(Format::formatString(processes[i].User())));
        processTable->setItem(i, 2, new QTableWidgetItem(Format::formatDouble(processes[i].CpuUtilization())));
        processTable->setItem(i, 3, new QTableWidgetItem(Format::formatString(processes[i].Ram())));
        processTable->setItem(i, 4, new QTableWidgetItem(Format::ElapsedTime(processes[i].UpTime())));
        processTable->setItem(i, 5, new QTableWidgetItem(QString::number(processes[i].Threads())));
        processTable->setItem(i, 6, new QTableWidgetItem(Format::formatString(processes[i].Command())));
    }
    filterProcesses(currentSearchText);
}

void TaskManager::filterProcesses(const QString &searchText) {
    for (int i = 0; i < processTable->rowCount(); ++i) {
        bool matches = false;
        for (int j = 0; j < processTable->columnCount(); ++j) {
            QTableWidgetItem *item = processTable->item(i, j);
            if (item && item->text().contains(searchText, Qt::CaseInsensitive)) {
                matches = true;
                break;
            }
        }
        processTable->setRowHidden(i, !matches);
    }
}

void TaskManager::contextMenuEvent(QContextMenuEvent *event) {
    QPoint pos = event->pos();
    QTableWidgetItem *item = processTable->itemAt(pos);

    if (item) {
        QMenu contextMenu(this);
        QAction *terminateAction = contextMenu.addAction("Завершить процесс (SIGTERM)");
        QAction *hangupAction = contextMenu.addAction("Перезапустить процесс (SIGHUP)");
        QAction *stopAction = contextMenu.addAction("Остановить процесс (SIGSTOP)");

        connect(terminateAction, &QAction::triggered, this, &TaskManager::terminateSelectedProcess);
        connect(hangupAction, &QAction::triggered, this, &TaskManager::hangupSelectedProcess);
        connect(stopAction, &QAction::triggered, this, &TaskManager::stopSelectedProcess);

        contextMenu.exec(event->globalPos());
    }
}

void TaskManager::sortTable(int column) {
    ascendingSort[column] = !ascendingSort[column];
    system->comp = ascendingSort[column];
    columnId = column;
    loadProcessData();
}
void TaskManager::terminateSelectedProcess() {
    int currentRow = processTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Ошибка", "Выберите процесс для завершения.");
        return;
    }
    int pid = processTable->item(currentRow, 0)->text().toInt();
    if (sendSignal(pid,SIGTERM)) {
        QMessageBox::information(this, "Успех", QString("Процесс с PID %1 был завершен.").arg(pid));
    } else {
        QMessageBox::warning(this, "Ошибка", QString("Не удалось завершить процесс с PID %1.").arg(pid));
    }
    loadProcessData();
}

void TaskManager::hangupSelectedProcess() {
    int currentRow = processTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Ошибка", "Выберите процесс для перезапуска.");
        return;
    }
    int pid = processTable->item(currentRow, 0)->text().toInt();
    if (sendSignal(pid, SIGHUP)) {
        QMessageBox::information(this, "Успех", QString("Сигнал SIGHUP был отправлен процессу с PID %1.").arg(pid));
    } else {
        QMessageBox::warning(this, "Ошибка", QString("Не удалось отправить сигнал SIGHUP процессу с PID %1.").arg(pid));
    }
    loadProcessData();
}

void TaskManager::stopSelectedProcess() {
    int currentRow = processTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Ошибка", "Выберите процесс для остановки.");
        return;
    }
    int pid = processTable->item(currentRow, 0)->text().toInt();
    if (sendSignal(pid, SIGSTOP)) {
        QMessageBox::information(this, "Успех", QString("Процесс с PID %1 был остановлен.").arg(pid));
    } else {
        QMessageBox::warning(this, "Ошибка", QString("Не удалось остановить процесс с PID %1.").arg(pid));
    }
    loadProcessData();
}

bool TaskManager::sendSignal(int pid, int signal){
    return kill(pid, signal) == 0;
}

bool TaskManager::terminateProcess(int pid) {
    if (kill(pid, SIGTERM) == 0) {
        return true;
    } else {
        return false;
    }
}
