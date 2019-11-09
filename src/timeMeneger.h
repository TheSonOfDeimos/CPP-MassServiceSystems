#ifndef timeMeneger_h
#define timeMeneger_h

#include <QObject>
#include <iostream>

#include "requestPool.h"
#include "devicePool.h"
#include "buffer.h"
#include "statisticManeger.hpp"

class TimeMeneger
{
public:
    
    TimeMeneger(SourcePool* sp, Buffer* buff, DevicePool* dp)
    : source_pool_handl_(sp),
    buffer_handl_(buff),
    device_pool_handl_(dp)
    {
    }
    
    void loopIteration()
    {
        auto request = source_pool_handl_ -> popRequest();
        if (request != nullptr) {
            if (device_pool_handl_ -> isFreeDevice(request.get())) {
                device_pool_handl_ -> chooseDevice(buffer_handl_ -> popReqest());
            }

            buffer_handl_ -> insert(std::move(request));
        }
    }
    
private:
    
    SourcePool* source_pool_handl_;
    Buffer* buffer_handl_;
    DevicePool* device_pool_handl_;
    
};


#endif
