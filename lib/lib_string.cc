#include "common.h"
#include <cstring>

using strVector_t = std::vector<std::string>;
// tokenize a string, delimiter a char
void tokenizeString(const std::string& src, 
                          const char* delimiter, 
                          strVector_t& result) {
    char* tmp = new char[src.size()+1];
    strcpy(tmp, src.c_str());
    char* token = strtok(tmp, delimiter);
    while(token!=nullptr) {
        result.push_back(std::string(token));
        token = strtok(NULL, delimiter);
    }
    delete[] tmp;
}

