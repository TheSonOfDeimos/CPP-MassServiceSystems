#include "mainwindow.h"
#include "autoModeWidget.hpp"
#include "configureWidget.hpp"
#include "timeMeneger.h"
#include "stepmodewidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    initConfigureWidget();
    initAutoModeWidget();
    initStepModeWidget();
    initTabs();
    
    mainWidget_ = new QWidget();
    setCentralWidget(tabs_);
    setMinimumSize({1250, 720});
}

MainWindow::~MainWindow()
{
}

void MainWindow::startAutoMode()
{    
    for (int i = 0; i < param_ -> num_of_req_; i++)
    {
        tm_ -> loopIteration();
    }   
}

void MainWindow::startStepMode()
{
    for (int i = 0; i < reinterpret_cast< StepModeWidget* >(step_mode_widget_) -> getStep(); i++)
    {
        tm_ -> loopIteration();

    }
}

void MainWindow::initModel()
{
    param_ = new Parametrs(reinterpret_cast< ConfigureWidget* >(configure_widget_) -> getParametrs());
    sp_ = new SourcePool(param_ -> num_of_sources_, param_ -> alpha_, param_ -> beta_);
    b_ = new Buffer(param_ -> size_of_buffer_);
    dp_ = new DevicePool(param_ -> num_of_devices_, param_ -> lambda_);

    sm_ = new StatisticManeger(sp_, b_, dp_);
    tm_ = new TimeMeneger(sp_, b_, dp_);

    QObject* obj = QObject::sender();
    if (obj == step_mode_widget_) {
        reinterpret_cast< StepModeWidget* >(step_mode_widget_) -> addModules(sp_, b_, dp_);
    }

    if (obj == auto_mode_widget_) {
        reinterpret_cast< AutoModeWidget* >(auto_mode_widget_) -> addStatisticManeger(sm_);
    }
}

void MainWindow::initTabs()
{
    tabs_ = new QTabWidget();
    tabs_ -> insertTab(0, configure_widget_, "Configure");
    tabs_ -> insertTab(1, auto_mode_widget_, "Auto mode");
    tabs_ -> insertTab(2, step_mode_widget_, "Step mode");
}

void MainWindow::initConfigureWidget()
{
    configure_widget_ = new ConfigureWidget();
}

void MainWindow::initAutoModeWidget()
{
    auto_mode_widget_ = new AutoModeWidget();
    connect(auto_mode_widget_, SIGNAL(start_signal()), this, SLOT(startAutoMode()));
    connect(auto_mode_widget_, SIGNAL(init_signal()), this, SLOT(initModel()));
}

void MainWindow::initStepModeWidget()
{
    step_mode_widget_ = new StepModeWidget(this);
    connect(step_mode_widget_, SIGNAL(init_signal()), this, SLOT(initModel()));
    connect(step_mode_widget_, SIGNAL(stepNext()), this, SLOT(startStepMode()));
}
