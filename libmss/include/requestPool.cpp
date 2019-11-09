#include "requestPool.h"

using namespace mss;

Request::Request()
    : time_in_(-1),
    time_in_buff_(-1),
    time_out_buff_(-1),
    time_out_(-1),
    source_id_(-1),
    device_id_(-1)
    {}
    
Request::Request(double time_in, double time_in_buff, double time_out_buff, double time_out, int source_id, int device_id)
    : time_in_(time_in),
    time_in_buff_(time_in_buff),
    time_out_buff_(time_out_buff),
    time_out_(time_out),
    source_id_(source_id),
    device_id_(device_id)
    {}

SourseOfRequest::SourseOfRequest()
: req_ptr_(nullptr),
id_(0),
time_(0),
counter_(0)
{
}

SourseOfRequest::SourseOfRequest(int source_id)
    : id_(source_id),
    time_(0),
    counter_(0)
    {
        srand(time(0));
        Request rq(time_, 0, 0, 0, id_, 0);
        req_ptr_ = std::make_unique< Request >(rq);
    }

SourseOfRequest::SourseOfRequest(SourseOfRequest& source)
: id_(source.id_),
time_(source.time_),
counter_(source.counter_)
{
    Request req = *source.req_ptr_.get();
    req_ptr_ = std::make_unique< Request >(req);
}

SourseOfRequest::SourseOfRequest(SourseOfRequest&& source) noexcept
: req_ptr_(std::move(source.req_ptr_)),
id_(std::move(source.id_)),
time_(std::move(source.time_)),
counter_(std::move(source.counter_))
{
}


SourseOfRequest::~SourseOfRequest()
{
}
    
    std::unique_ptr< Request > SourseOfRequest::release()
    {
        std::unique_ptr< Request > ptr = std::make_unique< Request >(generate());
        req_ptr_ -> time_in_buff_ = req_ptr_ -> time_in_;
        req_ptr_.swap(ptr);
        counter_++;
        return ptr;
    }
    
    const Request* SourseOfRequest::view() const
    {
        return req_ptr_.get();
    }

    Request SourseOfRequest::generate()
    {
        time_ += 1 / static_cast< double >(std::rand() % 10 + 1);
        return {time_, 0, 0, 0, id_, 0};
    }

int SourseOfRequest::getCounter() const
{
    return counter_;
}

int SourseOfRequest::getId() const
{
    return id_;
}

double SourseOfRequest::getTime() const
{
    return time_;
}

SourcePool::SourcePool()
: pool_(),
counter_(0)
{
}

SourcePool::SourcePool(int source_val)
: counter_(0)
    {
        for (int i = 0; i < source_val; ++i)
        {
            pool_.emplace_back(i);
        }
    }

SourcePool::~SourcePool()
{
}

void SourcePool::addSource(SourseOfRequest& source)
{
    pool_.push_back(SourseOfRequest(source));
}

    std::unique_ptr< Request > SourcePool::popRequest()
    {
        auto source = std::min_element(pool_.begin(), pool_.end(), [](SourseOfRequest& lhs, SourseOfRequest& rhs)
                                       {
                                           return lhs.view() -> time_in_ < rhs.view() -> time_in_;
                                       });
        
        auto res = source->release();
        counter_++;
        emit releaseNotyfy(res.get());
        return res;
    }
    
    int SourcePool::size() const
    {
        return pool_.size();
    }

int SourcePool::getCounter() const
{
    return counter_;
}

    int SourcePool::getCounter(int source_index) const
    {
        return pool_.at(source_index).getCounter();
    }
    
const SourseOfRequest* SourcePool::view(int source_index) const
{
    return &pool_.at(source_index);
}
