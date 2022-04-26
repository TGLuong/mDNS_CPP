#include <dns_sd.h>
#include <iostream>


void b(std::string j = "10", int a) {
    std::cout << j << std::endl;
}

int main() {
    b(NULL, 10);
    return 0;
}