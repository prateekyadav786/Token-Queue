#ifndef TOKEN_H
#define TOKEN_H

#include <string>

struct Token
{
    int id;
    std::string data;
    std::string status;
    int priority;

    Token(int i = 0,
          const std::string& d = "",
          int p = 2)
        : id(i), data(d), status("PENDING"), priority(p) {}
};

#endif

