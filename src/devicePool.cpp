#include "devicePool.h"
#include "requestPool.h"

#include <cmath>
#include <iostream>

Device::Device()
: req_ptr_(nullptr),
id_(0),
time_(0),
counter_(0),
lambda_(0)
{}

Device::Device(int id_device, double lambda)
: req_ptr_(nullptr),
id_(id_device),
    time_(0),
    counter_(0),
    lambda_(lambda)

    {
        
    }

Device::Device(Device& device)
: id_(device.id_),
time_(device.time_),
counter_(device.counter_),
  lambda_(device.lambda_)
{
    Request req = *device.req_ptr_.get();
    req_ptr_ = std::make_unique< Request >(req);
}

Device::Device(Device&& device)
: req_ptr_(std::move(device.req_ptr_)),
id_(std::move(device.id_)),
time_(std::move(device.time_)),
counter_(std::move(device.counter_)),
lambda_(std::move(device.lambda_))
{
}

Device::~Device()
{
}
    
    void Device::insert(std::unique_ptr< Request > req)
    {
        req_ptr_.release();
        req_ptr_ = std::move(req);
        
        //double waitTime =  1 / static_cast< double >(std::rand() % 10 + 1);;
        double waitTime = std::log(std::rand() % 1000 + 1) - std::log(1000) / (-lambda_);
        
        req_ptr_ -> device_id_ = id_;
        if (req_ptr_ -> time_in_ > time_)
        {
            time_ = req_ptr_ -> time_in_;
            req_ptr_ -> time_out_buff_ = time_;
            time_ += waitTime;
            req_ptr_ -> time_out_ = time_;
        } else
        {
            req_ptr_ -> time_out_buff_ = time_;
            time_ += waitTime;
            req_ptr_ -> time_out_ =  req_ptr_ -> time_in_ + (time_ - req_ptr_ -> time_in_);
        }
    }
    
    std::unique_ptr< Request > Device::release()
    {
        counter_++;
        return std::move(req_ptr_);
    }
    
    bool Device::isFree() const
    {
        return req_ptr_.get() == nullptr;
    }
    
    const Request* Device::view() const
    {
        return req_ptr_.get();
    }

int Device::getCounter() const
{
    return counter_;
}

int Device::getId() const
{
    return id_;
}

double Device::getTime() const
{
    return time_;
}
    


DevicePool::DevicePool()
: pool_(),
counter_(0)
{
}

DevicePool::DevicePool(int device_val, double lambda)
: counter_(0)
    {
        for (int i = 0; i < device_val; ++i)
        {
            pool_.emplace_back(i, lambda);
        }
    }

DevicePool::~DevicePool()
{
}

void DevicePool::addDevice(Device& device)
{
    pool_.push_back(Device(device));
}

const Device* DevicePool::view(int device_index) const
{
    return &pool_.at(device_index);
}
    
    bool DevicePool::isFreeDevice(const Request* req)
    {
        freeDevices(req);
        auto iter = std::find_if(pool_.begin(), pool_.end(), [](Device& dev)
                                 {
                                     return dev.isFree();
                                 });
        return iter != pool_.end();
    }
    
    void DevicePool::chooseDevice(std::unique_ptr< Request > req)
    {
        if (req == nullptr) {
            return;
        }
        
        auto iter = std::find_if(pool_.begin(), pool_.end(), [](Device& dev)
                                 {
                                     return dev.isFree();
                                 });
        
        
        iter -> insert(std::move(req));
        emit acceptedNotyfy(iter -> view());
        
    }
    
    double DevicePool::getTime(int device_index) const
    {
        return pool_.at(device_index).getTime();
    }
    
    int DevicePool::getCounter(int device_index) const
    {
        return pool_.at(device_index).getCounter();
    }

int DevicePool::getCounter() const
{
    return counter_;
}

    int DevicePool::size()
    {
        return pool_.size();
    }
    

    void DevicePool::freeDevices(const Request* req)
    {
        
        std::for_each(pool_.begin(), pool_.end(), [&req, this](Device& dev)
                      {
                          if (dev.view() != nullptr && req -> time_in_ > dev.view() -> time_out_)
                          {
                              counter_++;
                              auto r = dev.release();
                              emit releaseNotyfy(r.get());
                          }
                      });
    }

