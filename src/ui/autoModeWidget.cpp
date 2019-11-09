#include "autoModeWidget.hpp"
#include <QLabel>
#include <QString>

AutoModeWidget::AutoModeWidget()
{
    QVBoxLayout* vbl = new QVBoxLayout();
    setLayout(vbl);
    
    initResourceTable();
    initBufferTable();
    initDeviceTable();
    start_button_ = new QPushButton("Start simulation");
    init_button_ = new QPushButton("Init model");
    connect(start_button_, SIGNAL(clicked(bool)), this, SIGNAL(start_signal()));
    connect(init_button_, SIGNAL(clicked(bool)), this, SIGNAL(init_signal()));
    
    vbl -> addWidget(new QLabel("Source table"));
    vbl -> addWidget(source_table_);
    
    vbl -> addWidget(new QLabel("Buffer table"));
    vbl -> addWidget(buffer_table_);
    
    vbl -> addWidget(new QLabel("Device table"));
    vbl -> addWidget(device_table_);
    
    vbl -> addWidget(init_button_);
    vbl -> addWidget(start_button_);

    
    
}


void AutoModeWidget::initResourceTable()
{
    source_table_ = new QTableWidget();
    source_table_ -> setColumnCount(8);
    source_table_ -> setHorizontalHeaderLabels({"Request genereted", "Request accepted", "Request rejected", "Waiting time", "Time in system", "Total despersion", "Buffer despersion", "Reject probability"});
}

void AutoModeWidget::initBufferTable()
{
    buffer_table_ = new QTableWidget();
    buffer_table_ -> setWindowTitle("Buffer table");
    buffer_table_ -> setColumnCount(2);
    buffer_table_ -> setHorizontalHeaderLabels({"Request accepted", "Request rejected"});
    
}

void AutoModeWidget::initDeviceTable()
{
    device_table_ = new QTableWidget();
    device_table_ -> setColumnCount(4);
    device_table_ -> setHorizontalHeaderLabels({"Request accepted", "Request served", "Serve time", "Load factor"});
}

void AutoModeWidget::addDataSourceTable()
{
    SourceInfo info = statistic_ -> getSourceInfo(nullptr);
    source_table_ -> setRowCount(info.count_accepted_.size());
    for (int i = 0; i < info.count_accepted_.size(); i++)
    {
        source_table_ -> setVerticalHeaderItem(i, new QTableWidgetItem(QString("Source "  + QString::number(i))));
        
        source_table_ -> setItem(i, 0, new QTableWidgetItem(QString::number(info.count_summary_.at(i))));
        source_table_ -> setItem(i, 1, new QTableWidgetItem(QString::number(info.count_accepted_.at(i))));
        source_table_ -> setItem(i, 2, new QTableWidgetItem(QString::number(info.count_rejected_.at(i))));
        source_table_ -> setItem(i, 3, new QTableWidgetItem(QString::number(info.waiting_time_.at(i) / info.count_accepted_.at(i))));
        source_table_ -> setItem(i, 4, new QTableWidgetItem(QString::number(info.time_in_system_.at(i) / info.count_accepted_.at(i))));
        
        source_table_ -> setItem(i, 5, new QTableWidgetItem(QString::number((info.summary_square_time.at(i) * 1.0 / info.count_summary_.at(i)) - pow(info.time_in_system_.at(i) * 1.0 / info.count_summary_.at(i), 2))));
        source_table_ -> setItem(i, 6, new QTableWidgetItem(QString::number((info.summary_buff_square_time.at(i) * 1.0 / info.count_summary_.at(i)) - pow(info.waiting_time_.at(i) * 1.0 / info.count_summary_.at(i), 2))));
        
        source_table_ -> setItem(i, 7, new QTableWidgetItem(QString::number(((double)info.count_rejected_.at(i)) / ((double)info.count_summary_.at(i) + 1) * 100) + "%"));
    }
}

void AutoModeWidget::addDataBufferTable()
{
    BufferInfo info = statistic_ -> getBufferInfo(nullptr);
    buffer_table_ -> setRowCount(1);
    
    buffer_table_ -> setVerticalHeaderItem(0, new QTableWidgetItem(QString("Buffer")));
    buffer_table_ -> setItem(0, 0, new QTableWidgetItem(QString::number(info.count_accepted_)));
    buffer_table_ -> setItem(0, 1, new QTableWidgetItem(QString::number(info.count_rejected_)));
}

void AutoModeWidget::addDataDeviceTable()
{
    DeviceInfo info = statistic_ -> getDeviceInfo(nullptr);
    SourceInfo info2 = statistic_ -> getSourceInfo(nullptr);
    device_table_ -> setRowCount(info.service_count_.size());
    for (std::size_t i = 0; i < info.service_count_.size(); i++)
    {
        device_table_ -> setVerticalHeaderItem(i, new QTableWidgetItem(QString("Device "  + QString::number(i))));
        device_table_ -> setItem(i, 0, new QTableWidgetItem(QString::number(info.acceptance_count_.at(i))));
        device_table_ -> setItem(i, 1, new QTableWidgetItem(QString::number(info.service_count_.at(i))));
        device_table_ -> setItem(i, 2, new QTableWidgetItem(QString::number(info.service_time_.at(i) / info.acceptance_count_.at(i))));
        
        if (info.time_.at(i) > 0) {
            device_table_ -> setItem(i, 3, new QTableWidgetItem(QString::number(info.service_time_.at(i) / info.time_.at(i) * 100) + "%"));
        }
    }
    
    
}

void AutoModeWidget::addStatisticManeger(StatisticManeger *statistic)
{
    statistic_ = statistic;
    connect(statistic_, SIGNAL(infoNotify(QuickInfo)), this, SLOT(update()));
}

void AutoModeWidget::update()
{
    addDataSourceTable();
    addDataBufferTable();
    addDataDeviceTable();
}
