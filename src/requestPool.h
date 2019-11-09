#ifndef requestPool_h
#define requestPool_h

#include <memory>
#include <vector>
#include <QObject>

struct Request
{
    Request();
    Request(double time_in, double time_in_buff, double time_out_buff, double time_out, int source_id, int device_id);
   
    double time_in_;
    double time_in_buff_;
    double time_out_buff_;
    double time_out_;
    
    int source_id_;
    int device_id_;
};

class SourseOfRequest
{
public:
    SourseOfRequest();
    SourseOfRequest(int source_id, double alpha, double betta);
    SourseOfRequest(SourseOfRequest& source);
    SourseOfRequest(SourseOfRequest&& source) noexcept;
    virtual ~SourseOfRequest();
    
    std::unique_ptr< Request > release();
    const Request* view() const;
    int getCounter() const;
    int getId() const;
    double getTime() const;
    void setRandom(double alpha, double betta);
    
protected:
    virtual double generate();

private:
    std::unique_ptr< Request > req_ptr_;
    int id_;
    double time_;
    int counter_;
    double alpha_;
    double betta_;
};

class Notification : public QObject
{
Q_OBJECT
public:
    void releaseNotify(const Request* req)
    {
        emit notify(req);
    }
    
signals:
    void notify(const Request* req);
};

class SourcePool
{
public:
    SourcePool();
    SourcePool(int source_val, double alpha, double betta);
    virtual ~SourcePool();
   
    virtual std::unique_ptr< Request > popRequest();
    void addSource(SourseOfRequest& source);
    int size() const;
    int getCounter() const;
    int getCounter(int source_index) const;
    const SourseOfRequest* view(int source_index) const;
    
    Notification* notify_ = new Notification;
    
    

private:
    std::vector< SourseOfRequest > pool_;
    int counter_;
};

#endif

