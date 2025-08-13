#include "boost/thread.hpp"
#include <map>

struct struct_handle_set
{
    HANDLE mSemaphore1;
    HANDLE *mEvent;
    HANDLE *mHandles;
    unsigned int mSize;
    unsigned int mCtr;
};


namespace Moho {

struct CWaitHandleSet
{
    boost::mutex mLock;
    struct_handle_set mHandleSet;
    boost::condition mObjectSender;
    int mCount;
    boost::condition mObjectReceiver;
};

struct FWaitHandleSet
{
    boost::mutex mLock;
    boost::condition mObjectSender;
    int *mObj;
    int v1;
    int v2;
    int v3;
    int v4;
    std::map<unk_t> mMap1;
    std::map<unk_t> mMap2;
    Moho::CVirtualFileSystem *mHandle;
    boost::thread_specific_ptr mThreadStateInd;
};


}
