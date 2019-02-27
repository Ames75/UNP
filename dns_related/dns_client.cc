#include "common.h"
#include <unordered_map>

using namespace std;
void usage() {
    cout<<"Usage: udp_echo_client <option> ip/name list" <<endl;
}

using intStringHashMap_t = unordered_map<int, string>;

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
        if(arg_values.find(c) == arg_values.end()) {
            arg_values[c] = std::string(optarg);
        } else {
            arg_values[c] += string(" ") + string(optarg);  
        }
    }
    return arg_values.size();
}

int main(int argc, char** argv) {
    CHECK_ARG(3);
    intStringHashMap_t arg_values;
    buildOptValMap(arg_values, argc, argv, "i:n:");
    printOptValMap(arg_values);
    return 0;
}
