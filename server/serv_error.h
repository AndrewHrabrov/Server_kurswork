#pragma once
#include <string>
#include <stdexcept>
#include <system_error>

class server_error : public std::system_error {
public:
    server_error(int code, const std::string& s) : std::system_error(code, std::generic_category(), s) {}
};

class auth_error: public std::runtime_error {
public:
    auth_error(const std::string& s) : std::runtime_error(s) {}
    auth_error(const char * s) : std::runtime_error(s) {}
};

class vector_error: public std::runtime_error {
public:
    vector_error(const std::string& s) : std::runtime_error(s) {}
    vector_error(const char * s) : std::runtime_error(s) {}
};

class FileError : public std::runtime_error {
public:
    FileError(const std::string& s) : std::runtime_error(s) {}
};