#ifndef configureWidget_hpp
#define configureWidget_hpp

#include <stdio.h>
#include <QWidget>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QLabel>

struct Parametrs
{
    int num_of_sources_;
    int size_of_buffer_;
    int num_of_devices_;
    int num_of_req_;
    double alpha_;
    double beta_;
    double lambda_;
};

class ConfigureWidget : public QWidget
{
    Q_OBJECT
public:

    ConfigureWidget();

    Parametrs getParametrs();
private:
    QSpinBox* num_of_sources_;
    QSpinBox* size_of_buffer_;
    QSpinBox* num_of_devices_;
    QSpinBox* num_of_req_;
    QDoubleSpinBox* alpha_;
    QDoubleSpinBox* beta_;
    QDoubleSpinBox* lambda_;
};


#endif
