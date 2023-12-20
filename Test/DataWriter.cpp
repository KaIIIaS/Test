#include "DataWriter.h"

void DataWriter::writeToLog(const std::string& message) {
    logfile.open("log.txt", std::ios::app);
    logfile << message << std::endl;
    logfile.close();
}
