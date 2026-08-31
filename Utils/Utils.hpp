#ifndef UTILS_UTILS_HPP_
#define UTILS_UTILS_HPP_

#include <cstdio>

class Utils
{
public:
    Utils() = default;
    ~Utils() = default;

    static void init();
    static void delayUs(uint16_t us);
};

#endif
