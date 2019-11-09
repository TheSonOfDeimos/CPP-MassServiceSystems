#ifndef buffer_h
#define buffer_h

#include <memory>
#include <vector>
#include <QObject>

class Request;

class Buffer : public QObject
{
    Q_OBJECT
public:
    
    Buffer();
    Buffer(int cap);
    virtual ~Buffer();
    
    virtual void insert(std::unique_ptr< Request > el);
    virtual std::unique_ptr< Request > popReqest();
    void setCapacity(int cap);
    int size() const;
    int capacity() const;
    int getCountAccept() const;
    int getCountReject() const;
    const Request* view(int npos) const;
   
signals:
    void rejectedNotyfy(const Request* req);
    void acceptedNotyfy(const Request* req);
    void releaseNotyfy(const Request* req);

    void accNotyfy(const Request* req);
    
private:
    std::vector< std::unique_ptr< Request > > buff_;
    std::vector< std::unique_ptr< Request > > package_;
    int counter_accept_;
    int counter_reject_;
    int capacity_;
};

#endif
