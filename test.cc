#include <stdio.h>
#include <unistd.h>
#include <dns_sd.h>

#include <map>
#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <algorithm>





int main() {
    std::map<std::string, std::string> a;

    a.insert(std::pair<std::string, std::string>("name", "luong"));
    a.insert(std::pair<std::string, std::string>("age", "21"));
    a.insert(std::pair<std::string, std::string>("class", "CT3D"));
    a.insert(std::pair<std::string, std::string>("academy", "KMA"));

    std::map<std::string, std::string>::iterator itr;

    for (itr = a.begin(); itr != a.end(); itr++) {
        printf("%s: %s\n", itr->first.data(), itr->second.data());
    }

    std::map<std::string, std::string>::iterator f = a.find("haha");

    if (f != a.end()){
        f->second = "mat ma";
    } else {
        a.insert(std::pair<std::string, std::string>("haha", "haha"));
    }
    

    for (itr = a.begin(); itr != a.end(); itr++) {
        printf("%s: %s\n", itr->first.data(), itr->second.data());
    }


    a.erase("f");

    for (itr = a.begin(); itr != a.end(); itr++) {
        printf("%s: %s\n", itr->first.data(), itr->second.data());
    }
    return 0;
}
