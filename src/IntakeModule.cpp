#include "../include/IntakeModule.h"
#include <stdexcept>

IntakeModule::IntakeModule() : nextId(1) {}

bool IntakeModule::validate(const std::string& data)
{
    if (data.empty()) return false;
    if (data.length() > 256) return false;

    bool hasNonSpace = false;
    for (char c : data)
    {
        if (c != ' ')
        {
            hasNonSpace = true;
            break;
        }
    }
    return hasNonSpace;
}

Token IntakeModule::createToken(const std::string& data, int priority)
{
    if (!validate(data))
    {
        throw std::invalid_argument("Invalid token data");
    }

    return Token(nextId++, data, priority);
}

int IntakeModule::getNextId() const
{
    return nextId;
}
