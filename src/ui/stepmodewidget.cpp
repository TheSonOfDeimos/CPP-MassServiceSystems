#include "stepmodewidget.h"

StepModeWidget::StepModeWidget(QWidget* parent)
    : QWidget(parent)
{
    vbl_ = new QVBoxLayout(this);
    hbl_ = new QHBoxLayout(this);

    next_step_button_ = new QPushButton("Next step", this);
    init_model_ = new QPushButton("Init model", this);
    step_count_box_ = new QSpinBox(this);
    plot_ = new QCustomPlot(this);

    vbl_-> addWidget(plot_);
    hbl_ -> addWidget(next_step_button_);
    hbl_ -> addWidget(init_model_);
    hbl_ -> addWidget(step_count_box_);
    vbl_ -> addItem(hbl_);
    setLayout(vbl_);


    connect(next_step_button_, SIGNAL(clicked()), this, SIGNAL(stepNext()));
    connect(init_model_, SIGNAL(clicked()), this, SIGNAL(init_signal()));
}

void StepModeWidget::initPlot()
{
    // sourses
    for (int i = 0; i < source_pool_ -> size(); i++) {
        plot_ -> addGraph();
        plot_->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 10));
        plot_ -> graph() -> setPen(QPen(QColor(0, 0, 255, 255)));
    }

    // buffer
    plot_ -> addGraph();
    plot_->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCrossSquare, 10));
    plot_ -> graph() -> setPen(QPen(QColor(0, 255, 0, 255)));

    // devices
    for (int i = 0; i < device_pool_ -> size(); i++) {
        plot_ -> addGraph();
        plot_->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 10));
        plot_ -> graph() -> setPen(QPen(QColor(22, 23, 56, 255)));
    }

    //reject
    plot_ -> addGraph();
    plot_->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross, 10));
    plot_ -> graph() -> setPen(QPen(QColor(255, 0, 0, 255)));

    plot_ -> yAxis -> setRange(0, (source_pool_ -> size() + device_pool_ -> size()) * 2 + 6);

    plot_ -> replot();
}

void StepModeWidget::addModules(SourcePool* sp, Buffer* b, DevicePool* dp)
{
    source_pool_ = sp;
    buffer_ = b;
    device_pool_ = dp;

    connect(source_pool_ -> notify_, SIGNAL(notify(const Request*)), this, SLOT(reqGenerated(const Request*)));

    connect(buffer_, SIGNAL(accNotyfy(const Request*)), this, SLOT(bufferAccepted(const Request*)));
    connect(device_pool_, SIGNAL(acceptedNotyfy(const Request*)), this, SLOT(bufferRelesed(const Request*)));
    connect(buffer_, SIGNAL(rejectedNotyfy(const Request*)), this, SLOT(rejected(const Request*)));

    connect(device_pool_, SIGNAL(releaseNotyfy(const Request*)), this, SLOT(deviceAccepted(const Request*)));
    connect(device_pool_, SIGNAL(releaseNotyfy(const Request*)), this, SLOT(deviceReleased(const Request*)));

    initPlot();
}

// SOURCE
void StepModeWidget::reqGenerated(const Request* req)
{
    plot_ -> graph(req -> source_id_) -> addData(req -> time_in_ * 2, (req -> source_id_ + 1) * 2);
    plot_ -> xAxis->setRange(0, req -> time_in_ * 2 + 2);

    QCPItemText *textItem = new QCPItemText(plot_);
    textItem->setText("[" + QString::number(req -> source_id_) + "] " + QString::number(req -> time_in_));
    textItem->position->setCoords(QPointF(req -> time_in_ * 2, (req -> source_id_ + 1) * 2 + 0.4));
    textItem->setFont(QFont(font().family(), 10));

    plot_ -> replot();

}

// BUFFER
void StepModeWidget::bufferAccepted(const Request* req)
{
    plot_ -> graph(source_pool_ -> size()) -> addData(req -> time_in_buff_ * 2, ((source_pool_ -> size() + 1) * 2));

    QCPItemText *textItem = new QCPItemText(plot_);
    textItem->setText("[" + QString::number(req -> source_id_) + "] " + QString::number(req -> time_in_));
    textItem->position->setCoords(QPointF(req -> time_in_buff_ * 2, ((source_pool_ -> size() + 1) * 2) + 0.4));
    textItem->setFont(QFont(font().family(), 10));

    //plot_ -> replot();
}

void StepModeWidget::bufferRelesed(const Request* req)
{
    //plot_ -> graph(source_pool_ -> size()) -> setPen(QPen(QColor(255, 255, 0, 255)));
    //plot_ -> graph(source_pool_ -> size()) -> addData(req -> time_out_buff_ * 2, (source_pool_ -> size() * 2 + 2));
    //plot_ -> xAxis->setRange(0, req -> time_out_buff_ * 2 + 2);
    //plot_ -> graph(source_pool_ -> size()) -> setPen(QPen(QColor(0, 255, 0, 255)));
    //plot_ -> replot();
}

// DEVICE
void StepModeWidget::deviceAccepted(const Request *req)
{
    plot_ -> graph(source_pool_ -> size() + req -> device_id_ + 1) -> addData(req -> time_out_buff_ * 2, ((source_pool_ -> size() + 1 + req -> device_id_ + 1) * 2));

    QCPItemText *textItem = new QCPItemText(plot_);
    textItem->setText("[" + QString::number(req -> source_id_) + "] " + QString::number(req -> time_in_));
    textItem->position->setCoords(QPointF(req -> time_out_buff_ * 2, ((source_pool_ -> size() + 1 + req -> device_id_ + 1) * 2) + 0.4));
    textItem->setFont(QFont(font().family(), 10));

    //plot_ -> replot();
}

void StepModeWidget::deviceReleased(const Request *req)
{
    //plot_ -> replot();
}



// RJECT
void StepModeWidget::rejected(const Request* req)
{
    plot_ -> graph(source_pool_ -> size() + device_pool_ -> size() + 1) -> addData(req -> time_out_buff_ * 2, (source_pool_ -> size() + 1 + device_pool_ -> size() + 1) * 2);

    QCPItemText *textItem = new QCPItemText(plot_);
    textItem->setText("[" + QString::number(req -> source_id_) + "] " + QString::number(req -> time_in_));
    textItem->position->setCoords(QPointF(req -> time_out_buff_ * 2, (source_pool_ -> size() + 1 + device_pool_ -> size() + 1) * 2 + 0.4));
    textItem->setFont(QFont(font().family(), 10));

    //plot_ -> replot();
}


int StepModeWidget::getStep() const
{
    return step_count_box_->value();
}

void StepModeWidget::update()
{
    reqGenerated(nullptr);
    bufferAccepted(nullptr);
}
