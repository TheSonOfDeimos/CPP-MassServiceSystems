#include "configureWidget.hpp"


    ConfigureWidget::ConfigureWidget()
    {
        QGridLayout* vbl = new QGridLayout();
        setLayout(vbl);
        
        num_of_sources_ = new QSpinBox();
        size_of_buffer_ = new QSpinBox();
        num_of_devices_ = new QSpinBox();
        num_of_req_ = new QSpinBox();
        num_of_req_ -> setMaximum(500000);
        
        alpha_ = new QDoubleSpinBox();
        beta_ = new QDoubleSpinBox();
        lambda_ = new QDoubleSpinBox();
        
        vbl -> addWidget(new QLabel("Num of sources"), 0, 0);
        vbl -> addWidget(num_of_sources_, 0, 1);
        
        vbl -> addWidget(new QLabel("Size of buffer"), 1, 0);
        vbl -> addWidget(size_of_buffer_ , 1, 1);
        
        vbl -> addWidget(new QLabel("Num of devices"), 2, 0);
        vbl -> addWidget(num_of_devices_, 2, 1);
        
        vbl -> addWidget(new QLabel("Num of requests"), 3, 0);
        vbl -> addWidget(num_of_req_, 3, 1);
        
        vbl -> addWidget(new QLabel("Alpha"), 4, 0);
        vbl -> addWidget(alpha_, 4, 1);
        
        vbl -> addWidget(new QLabel("Beta"), 5, 0);
        vbl -> addWidget(beta_, 5, 1);
        
        vbl -> addWidget(new QLabel("Lambda"), 6, 0);
        vbl -> addWidget(lambda_, 6, 1);
    }

Parametrs ConfigureWidget::getParametrs()
{
    Parametrs res =  {num_of_sources_ -> value(), size_of_buffer_ -> value(),
                      num_of_devices_ -> value(), num_of_req_ -> value(),
                      alpha_ -> value(), beta_ -> value(), lambda_ -> value()
                  };
    return res;
}
