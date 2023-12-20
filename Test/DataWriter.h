#pragma once

#include <iostream>
#include <fstream>
#include <string>

class DataWriter {
public:
    void writeToLog(const std::string& message);

private:
    std::ofstream logfile;
};

