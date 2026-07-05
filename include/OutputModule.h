#ifndef OUTPUTMODULE_H
#define OUTPUTMODULE_H

#include "Token.h"
#include <vector>

class OutputModule
{
private:
    std::vector<Token> processedLog;
    std::vector<Token> invalidLog;

public:
    void store(const Token& token);
    void displayResults() const;

    int getProcessedCount() const;
    int getInvalidCount() const;
};

#endif
