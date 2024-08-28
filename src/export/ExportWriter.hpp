#pragma once
#include <Data.hpp>

class ExportWriter{
public:
    /**
     * @return if the project is created successfully
     */
    static bool windowWriter(const Data& data, std::string& message);

private;
    int _indent = 0;
};