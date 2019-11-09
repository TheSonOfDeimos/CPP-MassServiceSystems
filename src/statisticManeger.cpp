#include "statisticManeger.hpp"
#include <cmath>

StatisticManeger::StatisticManeger(SourcePool* sp, Buffer* buff, DevicePool* dp)
    : source_pool_handl_(sp),
    buffer_handl_(buff),
    device_pool_handl_(dp),
    quick_info_(sp -> size(), dp -> size()),
    source_info_(sp -> size()),
    buffer_info_(buff -> capacity()),
    device_info_(dp -> size())
    {
        // source pool
        connect(source_pool_handl_->notify_, SIGNAL(notify(const Request*)), this, SLOT(getQuickInfo(const Request*)));
        connect(source_pool_handl_->notify_, SIGNAL(notify(const Request*)), this, SLOT(processSourceRelease(const Request*)));
        
        // buffer
        connect(buffer_handl_, SIGNAL(rejectedNotyfy(const Request*)), this, SLOT(getQuickInfo(const Request*)));
        connect(buffer_handl_, SIGNAL(acceptedNotyfy(const Request*)), this, SLOT(getQuickInfo(const Request*)));
        connect(buffer_handl_, SIGNAL(releaseNotyfy(const Request*)), this, SLOT(getQuickInfo(const Request*)));
        
        connect(buffer_handl_, SIGNAL(rejectedNotyfy(const Request*)), this, SLOT(processBufferRejection(const Request*)));
        connect(buffer_handl_, SIGNAL(acceptedNotyfy(const Request*)), this, SLOT(processBufferAcceptance(const Request*)));
        connect(buffer_handl_, SIGNAL(releaseNotyfy(const Request*)), this, SLOT(processBufferRelease(const Request*)));
        
        // device pool
        connect(device_pool_handl_, SIGNAL(acceptedNotyfy(const Request*)), this, SLOT(getQuickInfo(const Request*)));
        connect(device_pool_handl_, SIGNAL(releaseNotyfy(const Request*)), this, SLOT(getQuickInfo(const Request*)));
        
        connect(device_pool_handl_, SIGNAL(acceptedNotyfy(const Request*)), this, SLOT(processDeviceAcceptance(const Request*)));
        connect(device_pool_handl_, SIGNAL(releaseNotyfy(const Request*)), this, SLOT(processDeviceRelease(const Request*)));
        
    }
    
    

    QuickInfo StatisticManeger::getQuickInfo(const Request*)
    {
        // sorce pool
        for (auto i = 0; i < source_pool_handl_ -> size(); ++i)
        {
            quick_info_.source_of_req_count_.at(i) = source_pool_handl_ -> getCounter(i);
        }
        
        // buffer
        quick_info_.buff_size_ = buffer_handl_ -> size();
        quick_info_.buff_accepted_ = buffer_handl_ -> getCountAccept();
        quick_info_.buff_rejected_ = buffer_handl_ -> getCountReject();
        
        // device pool
        for (auto i = 0; i < device_pool_handl_ -> size(); ++i)
        {
            quick_info_.device_pool_count_.at(i) = device_pool_handl_ -> getCounter(i);
        }
        
        emit infoNotify(quick_info_);
        return quick_info_;
    }
    
    SourceInfo StatisticManeger::getSourceInfo(const Request*)
    {
        return source_info_;
    }
    
    BufferInfo StatisticManeger::getBufferInfo(const Request*)
    {
        return StatisticManeger::buffer_info_;
    }
    
    DeviceInfo StatisticManeger::getDeviceInfo(const Request*)
    {
        return device_info_;
    }
    



    void StatisticManeger::processBufferRejection(const Request* req)
    {
        source_info_.count_rejected_.at(req -> source_id_)++;
        buffer_info_.count_rejected_++;
    }
    
    void StatisticManeger::processBufferAcceptance(const Request* req)
    {
        source_info_.count_accepted_.at(req -> source_id_)++;
        buffer_info_.count_accepted_++;
        buffer_info_.size_++;
        
        buffer_info_.state_.clear();
        for (int i = 0; i < buffer_handl_ -> size(); i++)
        {
            buffer_info_.state_.push_back(*buffer_handl_ -> view(i));
        }
        
    }
    
    void StatisticManeger::processSourceRelease(const Request* req)
    {
        source_info_.count_summary_.at(req -> source_id_)++;
    }
    
    void StatisticManeger::processBufferRelease(const Request* req)
    {
        
        buffer_info_.size_--;
        buffer_info_.state_.clear();
        for (int i = 0; i < buffer_handl_ -> size(); i++)
        {
            buffer_info_.state_.push_back(*buffer_handl_ -> view(i));
        }
        
        
    }
    
    void StatisticManeger::processDeviceAcceptance(const Request* req)
    {
        device_info_.acceptance_count_.at(req -> device_id_)++;
        device_info_.time_.at(req -> device_id_) = device_pool_handl_ -> getTime(req -> device_id_);
        device_info_.state_.at(req -> device_id_) = *req;
        source_info_.waiting_time_.at(req -> source_id_) += req -> time_out_buff_ - req -> time_in_;
        
        source_info_.summary_buff_square_time.at(req -> source_id_) += pow(req -> time_out_buff_ - req -> time_in_buff_, 2);
        
    }
    
    void StatisticManeger::processDeviceRelease(const Request* req)
    {
        device_info_.service_count_.at(req -> device_id_)++;
        device_info_.time_.at(req -> device_id_) = device_pool_handl_ -> getTime(req -> device_id_);
        device_info_.state_.at(req -> device_id_) = {-1, -1, -1, -1, -1, -1};
        device_info_.service_time_.at(req -> device_id_) += (req -> time_out_ - req -> time_out_buff_);
        source_info_.time_in_system_.at(req -> source_id_) += req -> time_out_ - req -> time_in_;
        
        
        source_info_.summary_square_time.at(req -> source_id_) += pow(req -> time_out_ - req -> time_in_, 2);
        
    }

