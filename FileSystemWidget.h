#ifndef FILESYSTEMWIDGET_H
#define FILESYSTEMWIDGET_H

#include <QTableWidget>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
class FileSystemWidget : public QWidget {
    Q_OBJECT

public:
    explicit FileSystemWidget(QWidget *parent = nullptr);

private:
    void loadSystemInfo(QTableWidget *table);
    void loadFileSystemInfo(QTableWidget *table);
    QTableWidget *fileSystemTable;
};

#endif // FILESYSTEMWIDGET_H
