#pragma once

#include <random>

extern inline uint16_t RandomPort()
{
    return (std::rand() % (65535 - 1024 + 1)) + 1024;
}