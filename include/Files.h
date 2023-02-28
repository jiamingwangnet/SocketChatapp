#pragma once

#include <string>

std::string getFileName(std::string path, char delimiter = '/');
std::string getFileExtension(std::string path, char delimiter = '/');
// add index.html if its a directory
void parseToFile(std::string& path);