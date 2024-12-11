#ifndef SYSTEMSTATSWIDGET_H
#define SYSTEMSTATSWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QProcess>
#include <QChartView>
#include <QLineSeries>
#include "include/system.h"
#include "format.h"
QT_CHARTS_USE_NAMESPACE

class SystemStatsWidget : public QWidget {
    Q_OBJECT

public:
    explicit SystemStatsWidget(QWidget *parent = nullptr);
    void updateStats();

private:
    QLabel *cpuUsageLabel;
    QProgressBar *cpuUsageBar;
    QLabel *memoryUsageLabel;
    QProgressBar *memoryUsageBar;
    QLabel *uptimeLabel;
    QLabel *processCountLabel;
    QLabel *threadCountLabel;
    QLabel *runProcessControlLabel;
    QChart *cpuChart;
    QLineSeries *cpuSeries;
    int cpuDataCount;
    System* system;
};

#endif // SYSTEMSTATSWIDGET_H
