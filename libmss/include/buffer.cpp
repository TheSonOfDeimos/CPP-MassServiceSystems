#include "buffer.h"
#include "requestPool.h"

using namespace mss;

Buffer::Buffer()
: counter_accept_(0),
counter_reject_(0),
capacity_(0)
{}

Buffer::Buffer(int cap)
: counter_accept_(0),
counter_reject_(0),
capacity_(0)
{}

Buffer::~Buffer()
{
}

    void Buffer::insert(std::unique_ptr< Request > el)
    {
        if (buff_.size() < capacity_) {
            buff_.push_back(std::move(el));
            counter_accept_++;
            emit acceptedNotyfy(buff_.back().get());
        }
        else  {
            counter_reject_++;
            emit rejectedNotyfy(el.get());
        }
    }
    
    std::unique_ptr< Request > Buffer::popReqest()
    {
        if (buff_.empty()) {
            return nullptr;
        }
        
        auto iter = std::max_element(buff_.begin(), buff_.end(), [](std::unique_ptr< Request >& lhs, std::unique_ptr< Request >& rhs)
                                     {
                                         return lhs -> source_id_ < rhs -> source_id_;
                                     });
        std::unique_ptr< Request > req;
        req.swap(*iter);
        buff_.erase(iter);
        
        emit releaseNotyfy(req.get());
        
        return req;
    }

void Buffer::setCapacity(int cap)
{
    if (buff_.size() > cap)
    {
        buff_.resize(cap);
    }
    capacity_ = cap;
}

    int Buffer::size() const
    {
        return buff_.size();
    }
    
    int Buffer::capacity() const
    {
        return capacity_;
    }
    
    int Buffer::getCountAccept() const
    {
        return counter_accept_;
    }
    
    int Buffer::getCountReject() const
    {
        return counter_reject_;
    }
    
    const Request* Buffer::view(int npos) const
    {
        return buff_.at(npos).get();
    }
    
