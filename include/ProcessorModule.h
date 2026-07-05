#ifndef PROCESSORMODULE_H
#define PROCESSORMODULE_H

#include "Token.h"
#include <deque>

class ProcessorModule
{
private:
    std::deque<Token> buffer;
    size_t maxCapacity;
    int totalEnqueued;
    int totalDequeued;

public:
    ProcessorModule(size_t capacity = 1000);

    bool enqueue(const Token& token);
    bool dequeue(Token& out);
    bool peek(Token& out) const;

    size_t size() const;
    bool isEmpty() const;
    size_t capacity() const;

    int getTotalEnqueued() const;
    int getTotalDequeued() const;

    void viewBuffer() const;
};

#endif

