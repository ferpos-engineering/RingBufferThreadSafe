#include "stdafx.h"
#include "BinarySemaphore.h"

// *************************************************************************

bool BinarySemaphore::TryLockAndCopy(Message* from, Message* to)
{
    std::lock_guard<std::mutex> lock(mutex);

    if (count > 0)
    {
        return false;
    }

    memcpy(to, from, sizeof(*to));
    ++count;
    condition.notify_one();
    return true;
}

// *************************************************************************

bool BinarySemaphore::TryUnlockAndCopy(Message* from, Message* to)
{
    std::lock_guard<std::mutex> lock(mutex);
    if (count > 0)
    {
        memcpy(to, from, sizeof(*to));
        --count;
        return true;
    }
    return false;
}

// *************************************************************************

void BinarySemaphore::UnlockAndCopy(Message* from, Message* to)
{
    std::unique_lock<std::mutex> lock(mutex);
    while (count == 0)
    {
        condition.wait(lock);
    }

    memcpy(to, from, sizeof(*to));
    --count;
}

// *************************************************************************