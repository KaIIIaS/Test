#include "DataReader.h"

bool DataReader::openFile(const std::string& file_name) {
    file.open(file_name);

    if (!file) {
        return false;
    }

    return true;
}

std::string DataReader::readFile() {
    std::string file_input((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    return file_input;
}

void DataReader::closeFile() {
    file.close();
}
