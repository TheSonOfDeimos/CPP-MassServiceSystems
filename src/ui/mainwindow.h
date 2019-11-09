#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include "statisticManeger.hpp"
#include "requestPool.h"
#include "buffer.h"
#include "devicePool.h"
#include "timeMeneger.h"
#include "configureWidget.hpp"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
    void initTables();
    void initTabs();
    void initConfigureWidget();
    void initAutoModeWidget();
    void initStepModeWidget();

    
public slots:
    void startAutoMode();
    void startStepMode();
    void initModel();

private:
    QWidget* tables_;
    QTabWidget* tabs_;
    
    QWidget* configure_widget_;
    QWidget* auto_mode_widget_;
    QWidget* step_mode_widget_;
    
    QWidget* mainWidget_;

    SourcePool* sp_;
    Buffer* b_;
    DevicePool* dp_;
    TimeMeneger* tm_;
    StatisticManeger* sm_;
    Parametrs* param_;

};
#endif // MAINWINDOW_H
