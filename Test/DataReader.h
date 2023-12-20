#pragma once

#include <iostream>
#include <fstream>
#include <string>

class DataReader {
public:
    bool openFile(const std::string& file_name);
    std::string readFile();
    void closeFile();

private:
    std::ifstream file;
};

