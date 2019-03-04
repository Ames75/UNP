#include "common.h"
#include <unordered_map>
#include <netdb.h>

using namespace std;

using charFuncMap_t = unordered_map<char, void(*)(const vector<string> &)>;
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

void findIpForNames(const vector<string>& names) {
    char ipstr[INET_ADDRSTRLEN];
    for (auto& dname:names) {
        struct hostent *hptr;
        if ((hptr = gethostbyname(dname.c_str())) != nullptr) {
           cout << "official name is " << hptr->h_name << endl;
           char** pptr = hptr->h_aliases;
           cout << "alias names: ";
           while(*pptr != nullptr) {
                cout << *pptr++ << "; ";
           }
           cout << endl;
           switch (hptr->h_addrtype) {
                case AF_INET:
                    pptr = hptr->h_addr_list;
                    cout << "ips: ";
                    while(*pptr != nullptr) {
                        const char* result = inet_ntop(hptr->h_addrtype, 
                                      (*pptr++),
                                      ipstr,sizeof(ipstr));
                        cout << std::string(result) << "; ";
                    }
                    cout << endl;
                    break;
                default:
                    cout << "unsupported addr type" << endl;
                    exit(1);
           }
        }
    }
}

void findNameForIps(const vector<string>& ips) {
    for (auto& ip:ips) {
        struct hostent *hptr;
        cout << " queried ip is " << ip << endl;
        struct in_addr addr; 
        if(my_inet_pton(AF_INET, ip.c_str(),&addr) ==0) {
            if ((hptr = gethostbyaddr(&addr, sizeof(addr), AF_INET)) 
                        != nullptr) {
                cout << "official name is " << hptr->h_name << endl;
                cout << "alias names: ";
                char** pptr = hptr->h_aliases;
                while(*pptr != nullptr) {
                     cout << *pptr++ << "; ";
                }
                cout << endl;
                char ipstr[INET_ADDRSTRLEN];
                switch (hptr->h_addrtype) {
                case AF_INET:
                    pptr = hptr->h_addr_list;
                    cout << "ips: ";
                    while(*pptr != nullptr) {
                        const char* result = inet_ntop(hptr->h_addrtype, 
                                      (*pptr++),
                                      ipstr,sizeof(ipstr));
                        cout << std::string(result) << "; ";
                    }
                    cout << endl;
                    break;
                default:
                    cout << "unsupported addr type" << endl;
                    exit(1);
           }
            }
        }
    }
}

class serventClass {
public:
    serventClass(struct servent* AServent):m_servent(AServent){};
    void print() {
        cout << "official service name: " << m_servent->s_name << endl;
        char** aliases = m_servent->s_aliases;
        cout << "aliases: ";
        while(*aliases != nullptr)  {
            cout << *aliases++ << "; ";
        }
    }
private:
     struct servent* m_servent;
};

void findServiceForPort( const vector<string>& ports) {
    for (auto& portStr:ports) {
        int portNum = std::stoi(portStr);
        struct servent* result;
        cout << "Port: " << portNum << endl;
        if((result = getservbyport(htons(portNum), NULL))!= nullptr) {
            serventClass(result).print();
        } else {
            handle_error("in findServiceForPort, after call to getservbyport");
        }
        cout << "*****************************" << endl;
    }
    
}

charFuncMap_t dnsCharFuncMap = {{'i', findNameForIps}, \
                                {'n', findIpForNames}, \
                                {'s', findServiceForPort}};

void run_service(intStringHashMap_t::value_type& entry) {
        vector<string> names;
        tokenizeString(entry.second," ", names);
        dnsCharFuncMap[entry.first](names);
}

int main(int argc, char** argv) {
    CHECK_ARG(3);
    intStringHashMap_t arg_values;
    buildOptValMap(arg_values, argc, argv, "i:n:s:");
    // printOptValMap(arg_values);
    for (auto& entry:arg_values) {
        run_service(entry);
    }
    return 0;
}
