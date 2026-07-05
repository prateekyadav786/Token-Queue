#ifndef INTAKEMODULE_H
#define INTAKEMODULE_H

#include "Token.h"
#include <string>

class IntakeModule
{
private:
    int nextId;

public:
    IntakeModule();

    bool validate(const std::string& data);

    Token createToken(const std::string& data,
                      int priority = 2);

    int getNextId() const;
};

#endif
