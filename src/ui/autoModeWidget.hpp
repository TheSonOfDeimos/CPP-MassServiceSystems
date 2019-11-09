#ifndef auto_mode_widget_hpp
#define auto_mode_widget_hpp

#include <QWidget>
#include <QPushButton>
#include "qcustomplot/qcustomplot.h"
#include "statisticManeger.hpp"


class AutoModeWidget : public QWidget
{
    Q_OBJECT
public:
    AutoModeWidget();
    
    void initResourceTable();
    void initBufferTable();
    void initDeviceTable();
    
    void addDataSourceTable();
    void addDataBufferTable();
    void addDataDeviceTable();
    
    void addStatisticManeger(StatisticManeger* statistic_);
   
    

public slots:
    void update();
    
signals:
    void start_signal();
    void init_signal();
    
private:
    QTableWidget*  source_table_;
    QTableWidget*  buffer_table_;
    QTableWidget*  device_table_;
    QPushButton* start_button_;
    QPushButton* init_button_;
    
    StatisticManeger* statistic_;
    
};

#endif /* table_view_hpp */
