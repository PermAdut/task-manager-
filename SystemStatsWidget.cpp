#include "SystemStatsWidget.h"
#include "ProcessManager.h"
#include <QVBoxLayout>
#include <QChart>
#include <QLineSeries>
#include <QValueAxis>
#include <QTimer>
#include <QDebug>
#include <dirent.h>
#include <cstring>
#include <cstdlib>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QStringList>

SystemStatsWidget::SystemStatsWidget(QWidget *parent)
    : QWidget(parent), system(new System()), cpuDataCount(0) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    cpuUsageLabel = new QLabel(this);
    cpuUsageBar = new QProgressBar(this);
    cpuUsageBar->setRange(0, 100);
    memoryUsageLabel = new QLabel(this);
    memoryUsageBar = new QProgressBar(this);
    memoryUsageBar->setRange(0, 100);
    uptimeLabel = new QLabel(this);
    processCountLabel = new QLabel(this);
    runProcessControlLabel = new QLabel(this);
    threadCountLabel = new QLabel(this);
    cpuChart = new QChart();
    cpuChart->setTitle("Нагрузка CPU (%)");
    auto cpuUsages = system->Cpu().Utilization();
    for (size_t i = 1; i < cpuUsages.size(); ++i) {
        QLineSeries *series = new QLineSeries();
        cpuChart->addSeries(series);
        series->setName(QString("CPU %1").arg(i));
    }
    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 100);
    cpuChart->addAxis(axisY, Qt::AlignLeft);
    QValueAxis *axisX = new QValueAxis();
    axisX->setRange(0, 60);
    cpuChart->addAxis(axisX, Qt::AlignBottom);
    for (auto series : cpuChart->series()) {
        series->attachAxis(axisY);
        series->attachAxis(axisX);
    }
    cpuChart->createDefaultAxes();
    cpuChart->legend()->show();
    QChartView *chartView = new QChartView(cpuChart);
    chartView->setMinimumHeight(200);
    layout->addWidget(cpuUsageLabel);
    layout->addWidget(cpuUsageBar);
    layout->addWidget(memoryUsageLabel);
    layout->addWidget(memoryUsageBar);
    layout->addWidget(uptimeLabel);
    layout->addWidget(processCountLabel);
    layout->addWidget(runProcessControlLabel);
    layout->addWidget(chartView);
    setLayout(layout);
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &SystemStatsWidget::updateStats);
    timer->start(1000);
}

void SystemStatsWidget::updateStats() {
    auto cpuUsages = system->Cpu().Utilization();
    cpuUsageLabel->setText(QString("Использование CPU: %1%").arg(cpuUsages[0], 0, 'f', 2));
    cpuUsageBar->setValue(static_cast<int>(cpuUsages[0]));

    for (size_t i = 1; i < cpuUsages.size(); ++i) {
        QLineSeries* series = qobject_cast<QLineSeries*>(cpuChart->series().at(i - 1));
        if (series) {
            series->append(cpuDataCount, cpuUsages[i]);
            series->setName(QString("CPU %1: %2%").arg(i).arg(cpuUsages[i], 0, 'f', 1));
        }
    }

    if (cpuDataCount >= 60) {
        for (auto& series : cpuChart->series()) {
            QLineSeries* lineSeries = qobject_cast<QLineSeries*>(series);
            if (lineSeries) {
                lineSeries->clear();
            }
        }
        cpuDataCount = 0;
    } else {
        cpuDataCount++;
    }

    double memoryUsage = system->MemoryUtilization();
    memoryUsageBar->setValue(static_cast<int>(memoryUsage));
    memoryUsageLabel->setText(QString("Использование памяти: %1%").arg(memoryUsage, 0, 'f', 2));
    QString formattedUptime = Format::ElapsedTime(system->UpTime());
    uptimeLabel->setText("Время работы системы: " + formattedUptime);
    processCountLabel->setText("Количество процессов: " + QString::number(system->TotalProcesses()));
    runProcessControlLabel->setText("Количество выполняющихся процессов: " + QString::number(system->RunningProcesses()));
}
