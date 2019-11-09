#ifndef STEPMODEWIDGET_H
#define STEPMODEWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSpinBox>
#include <qcustomplot/qcustomplot.h>
#include "timeMeneger.h"
#include "requestPool.h"
#include "devicePool.h"
#include "buffer.h"

class StepModeWidget : public QWidget
{
    Q_OBJECT
public:
    StepModeWidget(QWidget *parent);

    void initPlot();
    void addModules(SourcePool* sp, Buffer* b, DevicePool* dp);



public slots:
    void update();
    void reqGenerated(const Request *req);

    void deviceReleased(const Request *req);
    void deviceAccepted(const Request *req);

    void bufferAccepted(const Request* req);
    void bufferRelesed(const Request* req);
    void rejected(const Request* req);
    void setStepFlag()
    {
        flag = 1;
    }

    int getStep() const;

signals:
    void stepNext();
    void init_signal();

private:
    QVBoxLayout* vbl_;
    QHBoxLayout* hbl_;

    QPushButton* next_step_button_;
    QPushButton* init_model_;
    QSpinBox* step_count_box_;
    QCustomPlot* plot_;

    SourcePool* source_pool_;
    Buffer* buffer_;
    DevicePool* device_pool_;

    bool flag = 0;

};

#endif // STEPMODEWIDGET_H
