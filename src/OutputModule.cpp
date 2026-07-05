#include "../include/OutputModule.h"
#include <iostream>

void OutputModule::store(const Token& token)
{
    if (token.status == "PROCESSED")
        processedLog.push_back(token);
    else
        invalidLog.push_back(token);
}

void OutputModule::displayResults() const
{
    std::cout << "\n===== Processed Tokens =====\n";
    for (const auto& t : processedLog)
    {
        std::cout << "[" << t.id << "] "
                  << t.data << "\n";
    }

    std::cout << "\n===== Invalid Tokens =====\n";
    for (const auto& t : invalidLog)
    {
        std::cout << "[" << t.id << "] "
                  << t.data << "\n";
    }

    std::cout << "\nProcessed: " << processedLog.size() << "\n";
    std::cout << "Invalid : " << invalidLog.size() << "\n";
}

int OutputModule::getProcessedCount() const
{
    return processedLog.size();
}

int OutputModule::getInvalidCount() const
{
    return invalidLog.size();
}
