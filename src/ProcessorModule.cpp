#include "../include/ProcessorModule.h"
#include <iostream>
#include <iomanip>

ProcessorModule::ProcessorModule(size_t capacity)
    : maxCapacity(capacity), totalEnqueued(0), totalDequeued(0) {}

bool ProcessorModule::enqueue(const Token& token)
{
    if (buffer.size() >= maxCapacity)
        return false;

    buffer.push_back(token);
    totalEnqueued++;
    return true;
}

bool ProcessorModule::dequeue(Token& out)
{
    if (buffer.empty())
        return false;

    out = buffer.front();
    buffer.pop_front();
    totalDequeued++;
    return true;
}

bool ProcessorModule::peek(Token& out) const
{
    if (buffer.empty())
        return false;

    out = buffer.front();
    return true;
}

size_t ProcessorModule::size() const
{
    return buffer.size();
}

bool ProcessorModule::isEmpty() const
{
    return buffer.empty();
}

size_t ProcessorModule::capacity() const
{
    return maxCapacity;
}

int ProcessorModule::getTotalEnqueued() const
{
    return totalEnqueued;
}

int ProcessorModule::getTotalDequeued() const
{
    return totalDequeued;
}

void ProcessorModule::viewBuffer() const
{
    if (buffer.empty())
    {
        std::cout << "Queue is empty\n";
        return;
    }

    for (const auto& token : buffer)
    {
        std::cout << "[" << token.id << "] "
                  << token.data
                  << " (" << token.status << ")\n";
    }
}
