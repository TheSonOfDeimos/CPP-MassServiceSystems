#include "buffer.h"
#include "requestPool.h"

Buffer::Buffer()
: counter_accept_(0),
counter_reject_(0),
capacity_(0)
{}

Buffer::Buffer(int cap)
: counter_accept_(0),
counter_reject_(0),
capacity_(cap)
{}

Buffer::~Buffer()
{
}

    void Buffer::insert(std::unique_ptr< Request > el)
    {
        if (buff_.size() < capacity_) {
            buff_.push_back(std::move(el));
            emit accNotyfy(buff_.back().get());
        }

        else  {
            Request in = *el.get();
            auto place = std::find_if(buff_.rbegin(), buff_.rend(), [&in](std::unique_ptr< Request >& req)
                         {
                             return in.source_id_ > req -> source_id_;
                        });
            
            if (place != buff_.rend()) {
                Request r = *place -> get();
                place++;
                place.base() -> swap(el);
                counter_reject_++;
                r.time_out_buff_ = el.get() ->time_in_;
                emit rejectedNotyfy(&r);
            }
            else {
                counter_reject_++;
                el.get() -> time_out_buff_ = el.get() ->time_in_;
                emit rejectedNotyfy(el.get());
            }
        }
    }
    
    std::unique_ptr< Request > Buffer::popReqest()
    {
        if (package_.empty()) {
            if (buff_.empty()) {
                return nullptr;
            }
            
            auto iter = std::max_element(buff_.begin(), buff_.end(), [](std::unique_ptr< Request >& lhs, std::unique_ptr< Request >& rhs)
                                         {
                                             return lhs -> source_id_ < rhs -> source_id_;
                                         });
            
            for (std::size_t i = 0; i < buff_.size(); i++)
            {
                if (buff_.at(i) -> source_id_ == iter -> get() -> source_id_)
                {
                    std::unique_ptr< Request > req;
                    req.swap(buff_.at(i));
                    buff_.erase(buff_.begin() + i);
                    capacity_--;
                    package_.push_back(std::move(req));
                }
            }
        }
        
        auto iter = std::max_element(package_.begin(), package_.end(), [](std::unique_ptr< Request >& lhs, std::unique_ptr< Request >& rhs)
                                     {
                                         return lhs -> time_in_buff_ > rhs -> time_in_buff_;
                                     });
        
        std::unique_ptr< Request > req;
        req.swap(*iter);
        package_.erase(iter);
        capacity_++;
        counter_accept_++;
        emit acceptedNotyfy(req.get());
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
    
