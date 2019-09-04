#pragma once

#include <random>

extern inline short RandomPort()
{
    return (short)((std::rand() % (65535 - 1024 + 1)) + 1024);
}