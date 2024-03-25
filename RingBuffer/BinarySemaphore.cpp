#include "stdafx.h"
#include <chrono>
#include "BinarySemaphore.h"

// *************************************************************************

bool BinarySemaphore::LockAndCopy(Message* from, Message* to)
{
    lock_guard<std::mutex> lock(mutex);

    memcpy(to, from, sizeof(*to));

    if (count == 0)
    {
        ++count;
        condition.notify_one();
        return true;
    }

    return false;
}

// *************************************************************************

bool BinarySemaphore::TryUnlockAndCopy(
    Message* from, Message* to, unsigned int timeoutInMilliseconds)
{
    unique_lock<std::mutex> lock(mutex);
    while (count == 0)
    {
        if (condition.wait_for(
            lock,
            chrono::microseconds(timeoutInMilliseconds)) == cv_status::timeout)
        {
            return false;
        }
    }

    memcpy(to, from, sizeof(*to));
    --count;
    return true;
}

// *************************************************************************