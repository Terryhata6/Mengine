#include "ThreadMutexDummy.h"

namespace Mengine
{
    //////////////////////////////////////////////////////////////////////////
    void ThreadMutexDummy::lock()
    {
        //Empty
    };
    //////////////////////////////////////////////////////////////////////////
    void ThreadMutexDummy::unlock()
    {
        //Empty
    };
    //////////////////////////////////////////////////////////////////////////
    bool ThreadMutexDummy::try_lock()
    {
        //Empty

        return true;
    };
}