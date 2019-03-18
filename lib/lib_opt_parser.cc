#include "common.h"
using namespace std;
void printOptValMap(const intStringHashMap_t& optMap) {
    for (auto& elem : optMap) {
        char opt = elem.first;
        cout << "option is " << opt << endl;
        cout << "value is " << elem.second << endl;
    }
}

// return num of options captured
size_t buildOptValMap(intStringHashMap_t& arg_values, 
                            int argc, char** argv,
                            const char* optstr) {
    int c;
    while ((c = getopt(argc, argv, optstr)) != -1) {
        std::string optstr(optarg==NULL?"":optarg);
        if(arg_values.find(c) == arg_values.end()) {
            arg_values[c] = optstr;
        } else if (!arg_values[c].empty()){
            arg_values[c] += string(" ") + optstr;  
        }
    }
    return arg_values.size();
}


