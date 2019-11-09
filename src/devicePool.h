#ifndef devicePool_h
#define devicePool_h

#include <QObject>

class Request;

class Device
{
public:
    Device();
    Device(int id_device, double lambda);
    Device(Device& device);
    Device(Device&& device);
    virtual ~Device();
    
    void insert(std::unique_ptr< Request > req);
    std::unique_ptr< Request > release();
    bool isFree() const;
    const Request* view() const;
    int getCounter() const;
    int getId() const;
    double getTime() const;
    
private:
    std::unique_ptr< Request > req_ptr_;
    int id_;
    double time_;
    int counter_;
    double lambda_;
};

class DevicePool : public QObject
{
Q_OBJECT

public:
    DevicePool();
    DevicePool(int device_val, double lambda);
    virtual ~DevicePool();
    
    void addDevice(Device& device);
    const Device* view(int device_index) const;
    bool isFreeDevice(const Request* req);
    void chooseDevice(std::unique_ptr< Request > req);
    double getTime(int device_index) const;
    int getCounter(int device_index) const;
    int getCounter() const;
    int size();
    
signals:
    void releaseNotyfy(const Request* req);
    void acceptedNotyfy(const Request* req);
    
private:
    void freeDevices(const Request* req);
    std::vector< Device > pool_;
    int counter_;
};

#endif
