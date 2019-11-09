#ifndef statisticManeger_hpp
#define statisticManeger_hpp

#include <QObject>
#include <iostream>

#include "requestPool.h"
#include "devicePool.h"
#include "buffer.h"

struct QuickInfo
{
    QuickInfo(int source_num, int device_num)
    : source_of_req_count_(source_num, 0),
    device_pool_count_(device_num, 0),
    buff_size_(0),
    buff_accepted_(0),
    buff_rejected_(0)
    {
    }
    
    std::vector< int > source_of_req_count_;
    
    int buff_size_;
    int buff_accepted_;
    int buff_rejected_;
    
    std::vector< int > device_pool_count_;
};

struct SourceInfo
{
    SourceInfo(int source_num)
    : count_summary_(source_num, 0),
    count_accepted_(source_num, 0),
    count_rejected_(source_num, 0),
    waiting_time_(source_num, 0),
    time_in_system_(source_num, 0),
    summary_square_time(source_num, 0),
    summary_buff_square_time(source_num, 0)
    {}
    
    std::vector< int > count_summary_;
    std::vector< int > count_accepted_;
    std::vector< int > count_rejected_;
    std::vector< double > waiting_time_;
    std::vector< double > time_in_system_;
    
    std::vector< double > summary_square_time;
    std::vector< double > summary_buff_square_time;
};

struct BufferInfo
{
    BufferInfo(int buff_cap)
    : size_(0),
    count_accepted_(0),
    count_rejected_(0),
    state_(buff_cap)
    {}
    
    int size_;
    int count_accepted_;
    int count_rejected_;
    
    std::vector< Request > state_;
};

struct DeviceInfo
{
    DeviceInfo(int num_device)
    : service_count_(num_device, 0),
    acceptance_count_(num_device, 0),
    time_(num_device, 0),
    service_time_(num_device, 0),
    state_(num_device)
    
    {}
    
    std::vector< int > service_count_;
    std::vector< int > acceptance_count_;
    std::vector< double > time_;
    std::vector< double > service_time_;
    std::vector< Request > state_;
};

class StatisticManeger : public QObject
{
    Q_OBJECT
public:
    StatisticManeger(SourcePool* sp, Buffer* buff, DevicePool* dp);
    
public slots:
    QuickInfo getQuickInfo(const Request*);
    SourceInfo getSourceInfo(const Request*);
    BufferInfo getBufferInfo(const Request*);
    DeviceInfo getDeviceInfo(const Request*);
    
signals:
    void infoNotify(QuickInfo info);
    
private:
    SourcePool* source_pool_handl_;
    Buffer* buffer_handl_;
    DevicePool* device_pool_handl_;

    QuickInfo quick_info_;
    SourceInfo source_info_;
    BufferInfo buffer_info_;
    DeviceInfo device_info_;
    
private slots:
    void processBufferRejection(const Request* req);
    void processBufferAcceptance(const Request* req);
    void processSourceRelease(const Request* req);
    void processBufferRelease(const Request*);
    void processDeviceAcceptance(const Request* req);
    void processDeviceRelease(const Request* req);
};

#endif /* statisticManeger_hpp */
