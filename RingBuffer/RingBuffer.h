#pragma once

#include <mutex>
#include "Message.h"
#include "BinarySemaphore.h"

using namespace std;

// *************************************************************************
//! @brief    This class implements a thread safe ring buffer.
//!
//! @author   FP
//!
//
class RingBuffer
{
public:
    // *********************************************************************
    //! @brief    It instantiates an instance of this class.
    //!
    //! @param    elements  Number of elements of the buffer.
    //!
    RingBuffer(size_t elements);

    // *********************************************************************
    //! @brief    It finalizes allocated resources in this class.
    //!
    ~RingBuffer();

    // *********************************************************************
    //! @brief    It stores a Measure object in the buffer.
    //!
    //! @param    data  A pointer to the object to be copied in the buffer.
    //!
    //! @return   True if the message has been stored in the data buffer
    //!           without overwriting a location already full. Otherwise
    //!           false.
    //!
    bool Put(Message* data);

    // *********************************************************************
    //! @brief    It tries to get the next object available in the buffer.
    //!           A call to this method does not block the caller.
    //!
    //! @param    data  A pointer to the object in which the buffer element
    //!                 is copied.
    //!
    //! @return   True if the message has been found and extracted, false
    //!           otherwise.
    //!
    bool TryGet(Message* data);

    // *********************************************************************
    //! @brief    It gets the next object available in the buffer.
    //!           A call to this method blocks the caller.
    //!
    //! @param    data  A pointer to the object in which the buffer element
    //!                 is copied.
    //!
    void Get(Message* data);

private:

    // *********************************************************************
    //! @brief    The internal data structure.
    //!
    Message* buffer = nullptr;

    // *********************************************************************
    //! @brief    The buffer size (number of elements).
    //!
    size_t elements;

    // *********************************************************************
    //! @brief    Index of the producer.
    //!
    size_t putIndex;

    // *********************************************************************
    //! @brief    Index of the consumer.
    //!
    size_t getIndex;

    // *********************************************************************
    //! @brief    It protects the buffer put from race conditions.
    //!
    mutex  putLock;

    // *********************************************************************
    //! @brief    It protects the buffer get from race conditions.
    //!
    mutex  getLock;

    // *********************************************************************
    //! @brief    It synchronizes the access to single elements of the buffer.
    //!
    BinarySemaphore* semaphores = nullptr;
};