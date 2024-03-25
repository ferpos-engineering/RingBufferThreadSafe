// RingBuffer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "RingBuffer.h"

#include "RingBuffer.h"
#include "Message.h"

#define NUM_THREADS 10
#define NUM_ELEMENTS_RINGBUFFER 50
#define CONSUMER_ACCESS_TIMEOUT_IN_MS 5000u

int values[NUM_THREADS];
RingBuffer* ringBuffer;

void producer(int index)
{
    Message message;
    message.integer1 = index;
    message.integer2 = -(index);
    ringBuffer->Put(&message);
}

void consumer(int index)
{
    Message message;
    ringBuffer->TryGet(&message, CONSUMER_ACCESS_TIMEOUT_IN_MS);

    values[index] = message.integer1;
}

int main()
{
    memset(values, 0xff, sizeof(int) * NUM_THREADS);

    ringBuffer = new RingBuffer(NUM_ELEMENTS_RINGBUFFER);
    thread* producers[NUM_THREADS];
    thread* consumers[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++)
    {
        thread* thread_obj2 = new thread(consumer, i);
        consumers[i] = thread_obj2;
        thread* thread_obj1 = new thread(producer, i);
        producers[i] = thread_obj1;
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        consumers[i]->join();
        producers[i]->join();
    }

    int sum = 0;
    for (int i = 0; i < NUM_THREADS; i++)
    {
        sum += values[i];
        printf("Consumed[%d] = %d\r\n", i, values[i]);
    }

    printf("Total = %d\r\n", sum);

    for (int i = 0; i < NUM_THREADS; i++)
    {
        if (producers[i] != nullptr)
        {
            delete producers[i];
            producers[i] = nullptr;
        }

        if (consumers[i] != nullptr)
        {
            delete consumers[i];
            consumers[i] = nullptr;
        }
    }

    delete ringBuffer;

    return 0;
}