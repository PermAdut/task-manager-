#include "FileSystemWidget.h"
#include <QVBoxLayout>
#include <QTableWidgetItem>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QHeaderView>
#include <sys/vfs.h>
#include <QLabel>
#include <QTableWidget>
#include "include/system.h"
#include <QMessageBox>

FileSystemWidget::FileSystemWidget(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *systemInfoLabel = new QLabel("Информация о системе", this);
    layout->addWidget(systemInfoLabel);
    QTableWidget *cpuTable = new QTableWidget(this);
    cpuTable->setColumnCount(2);
    cpuTable->setHorizontalHeaderLabels(QStringList() << "Параметр" << "Значение");
    layout->addWidget(cpuTable);
    cpuTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    QLabel *fileSystemInfoLabel = new QLabel("Информация о файловой системе", this);
    layout->addWidget(fileSystemInfoLabel);
    QTableWidget *fileSystemTable = new QTableWidget(this);
    fileSystemTable->setColumnCount(2);
    fileSystemTable->setHorizontalHeaderLabels(QStringList() << "Параметр" << "Значение");
    layout->addWidget(fileSystemTable);
    fileSystemTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    loadSystemInfo(cpuTable);
    loadFileSystemInfo(fileSystemTable);
    cpuTable->resizeColumnToContents(0);
    cpuTable->resizeColumnToContents(1);
    fileSystemTable->resizeColumnToContents(0);
    fileSystemTable->resizeColumnToContents(1);
}

void FileSystemWidget::loadSystemInfo(QTableWidget *table) {
    std::string modelName;
    std::string cpuSpeed;
    std::string cacheSize;
    int coreCount = 0;
    std::ifstream file("/proc/cpuinfo");
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            if (line.find("model name") == 0) {
                std::istringstream linestream(line);
                std::string key;
                std::getline(linestream, key, ':');
                std::getline(linestream, modelName);
                modelName = modelName.substr(1);
            } else if (line.find("cpu MHz") == 0) {
                std::istringstream linestream(line);
                std::string key;
                std::getline(linestream, key, ':');
                std::getline(linestream, cpuSpeed);
                cpuSpeed = cpuSpeed.substr(1);
            } else if (line.find("cache size") == 0) {
                std::istringstream linestream(line);
                std::string key;
                std::getline(linestream, key, ':');
                std::getline(linestream, cacheSize);
                cacheSize = cacheSize.substr(1);
            } else if (line.find("processor") == 0) {
                coreCount++;
            }
        }
        file.close();
        table->insertRow(table->rowCount());
        table->setItem(table->rowCount() - 1, 0, new QTableWidgetItem("Модель"));
        table->setItem(table->rowCount() - 1, 1, new QTableWidgetItem(QString::fromStdString(modelName)));
        table->insertRow(table->rowCount());
        table->setItem(table->rowCount() - 1, 0, new QTableWidgetItem("Количество ядер"));
        table->setItem(table->rowCount() - 1, 1, new QTableWidgetItem(QString::number(coreCount)));
        table->insertRow(table->rowCount());
        double cpuSpeedGHz = std::stod(cpuSpeed) / 1000.0;
        table->setItem(table->rowCount() - 1, 0, new QTableWidgetItem("Частота (ГЦ)"));
        table->setItem(table->rowCount() - 1, 1, new QTableWidgetItem(QString::number(cpuSpeedGHz, 'f', 2)));
        table->insertRow(table->rowCount());
        table->setItem(table->rowCount() - 1, 0, new QTableWidgetItem("Кэш (кб)"));
        table->setItem(table->rowCount() - 1, 1, new QTableWidgetItem(QString::fromStdString(cacheSize)));
    }

    System* system = new System();
    QString kernel = QString::fromStdString(system->Kernel());
    QString operationSystem = QString::fromStdString(system->OperatingSystem());

    table->insertRow(table->rowCount());
    table->setItem(table->rowCount() - 1, 0, new QTableWidgetItem("Операционная система"));
    table->setItem(table->rowCount() - 1, 1, new QTableWidgetItem(operationSystem));

    table->insertRow(table->rowCount());
    table->setItem(table->rowCount() - 1, 0, new QTableWidgetItem("Ядро"));
    table->setItem(table->rowCount() - 1, 1, new QTableWidgetItem(kernel));
}

void FileSystemWidget::loadFileSystemInfo(QTableWidget *table) {
    // Получение информации о файловой системе
    struct statfs fsInfo;
    if (statfs("/", &fsInfo) == 0) {
        table->insertRow(table->rowCount());
        table->setItem(table->rowCount() - 1, 0, new QTableWidgetItem("Размер блока"));
        table->setItem(table->rowCount() - 1, 1, new QTableWidgetItem(QString::number(fsInfo.f_bsize)));

        table->insertRow(table->rowCount());
        table->setItem(table->rowCount() - 1, 0, new QTableWidgetItem("Общее количество блоков"));
        table->setItem(table->rowCount() - 1, 1, new QTableWidgetItem(QString::number(fsInfo.f_blocks)));

        table->insertRow(table->rowCount());
        table->setItem(table->rowCount() - 1, 0, new QTableWidgetItem("Доступные блоки"));
        table->setItem(table->rowCount() - 1, 1, new QTableWidgetItem(QString::number(fsInfo.f_bfree)));
    }
}
