#pragma once

#include <exception>
#include <stdexcept>
#include <string>

class NanoException : public std::runtime_error
{
public:
	NanoException(std::string msg) : std::runtime_error(msg) {}
};