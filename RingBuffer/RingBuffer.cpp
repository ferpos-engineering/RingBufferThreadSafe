#include "stdafx.h"
#include "RingBuffer.h"

// *************************************************************************

RingBuffer::RingBuffer(size_t elements)
{
    this->getIndex = 0;
    this->putIndex = 0;
    this->elements = elements;
    this->buffer = new Message[elements];
    this->semaphores = new BinarySemaphore[elements];
}

// *************************************************************************

RingBuffer::~RingBuffer()
{
    this->getIndex = 0;
    this->putIndex = 0;

    if (this->semaphores != nullptr)
    {
        delete[] this->semaphores;
    }

    if (this->buffer != nullptr)
    {
        delete[] this->buffer;
        this->buffer = nullptr;
    }

    this->elements = 0;
}

// *************************************************************************

bool RingBuffer::Put(Message* data)
{
    size_t current;

    // Mutex and we get the current put index, then we push the index forward.
    this->putLock.lock();
    current = this->putIndex;
    this->putIndex = (current + 1) % this->elements;
    this->putLock.unlock();

    // Try to write data to the current buffer element.
    if (!this->semaphores[current].LockAndCopy(data, &this->buffer[current]))
    {
        printf("Producers too fast, element is already full.\r\n");
        return false;
    }

    return true;
}

// *************************************************************************

bool RingBuffer::TryGet(Message* data, int timeoutInMilliseconds)
{
    size_t current;

    // Mutex and we get the current get index, then we push the index forward.
    this->getLock.lock();
    current = this->getIndex;
    this->getIndex = (current + 1) % this->elements;
    this->getLock.unlock();

    // Try to gather the current element in buffer and copy it to data.
    if (!this->semaphores[current].TryUnlockAndCopy(
        &this->buffer[current], data, timeoutInMilliseconds))
    {
        printf("Consumer not served, element cannot be retrieved.\r\n");
        return false;
    }

    return true;
}

// *************************************************************************