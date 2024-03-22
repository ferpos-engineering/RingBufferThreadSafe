#pragma once

#include <mutex>
#include <condition_variable>
#include "Message.h"

using namespace std;

// *************************************************************************
//! @brief    This class synchronizes the access to single elements of a
//!           buffer.
//!
//! @author   FP
//!
//
class BinarySemaphore
{
public:

    // *********************************************************************
    //! @brief    It copies the content of the source object to destination
    //!           if the internal lock is unlocked.
    //!
    //! @remarks  Calling this method does not block the execution of the
    //!           client code.
    //!           Calling this method locks the internal lock.
    //! 
    //! @param    from  Source of the copy.
    //!
    //! @param    to    Destination of the copy.
    //!
    //! @return   True  if success (internal lock become locked);
    //!                 false, the internal lock was already locked.
    //!
    bool TryLockAndCopy(Message* from, Message* to);

    // *********************************************************************
    //! @brief    It copies the content of the source object to destination
    //!           if the internal lock is locked.
    //!
    //! @remarks  Calling this method blocks the execution of the
    //!           client code.
    //!           Calling this method unlocks the internal lock.
    //! 
    //! @param    from  Source of the copy.
    //!
    //! @param    to    Destination of the copy.
    //!
    //! @return   True  if success (internal lock is unlocked);
    //!                 false, the internal lock was already unlocked.
    //! 
    void UnlockAndCopy(Message* from, Message* to);

    // *********************************************************************
    //! @brief    It copies the content of the source object to destination
    //!           if the internal lock is locked.
    //!
    //! @remarks  Calling this method blocks does not block the execution of the
    //!           client code.
    //!           Calling this method unlocks the internal lock.
    //! 
    //! @param    from  Source of the copy.
    //!
    //! @param    to    Destination of the copy.
    //!
    //! @return   True  if success (internal lock is unlocked);
    //!                 false, the internal lock was already unlocked.
    //! 
    bool TryUnlockAndCopy(Message* from, Message* to);

private:

    mutex mutex;
    condition_variable condition;

    // *********************************************************************
    //! @brief    The semaphore is initalized unlocked.
    //! 
    unsigned int count = 0; 
};