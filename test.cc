#include <stdio.h>
#include <unistd.h>
#include <dns_sd.h>

#include <map>
#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>



void print_vector(std::vector<std::string> vect) {
    for (std::string s : vect) {
        printf("%s ", s.data());
    }
    printf("\n");
}

int main() {
    std::vector<std::string> a = {"haha", "hihi", "gialuong"};

    print_vector(a);

    std::remove_if(a.begin(), a.end(), [](std::string s){
        if (s.compare("hihi") == 0) return true;
        return false;
    });

    print_vector(a);
    
    return 0;
}
