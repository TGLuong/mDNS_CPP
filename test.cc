#include <stdio.h>
#include <unistd.h>
#include <dns_sd.h>

#include <map>
#include <iostream>
#include <thread>
#include <vector>

int loop = 1;

void foo(int a, int b) {
    while (loop) {
        printf("aha\n");
        sleep(1);
    }
}

std::vector<std::string>



int main() {
    std::vector<std::string> a = {"haha", "hihi"};
    std::vector<std::string> b(a);

    b.push_back("luong");
    printf("%ld %ld\n", a.size(), b.size());
    return 0;
}
