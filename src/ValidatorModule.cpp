#include "../include/ValidatorModule.h"

Token ValidatorModule::process(Token token)
{
    if (token.data.length() >= 3 &&
        token.data.substr(0, 3) == "ERR")
    {
        token.status = "INVALID";
    }
    else
    {
        token.status = "PROCESSED";
    }

    return token;
}
