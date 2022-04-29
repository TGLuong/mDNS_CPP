#include "mdnssub.h"
#include <stdio.h>
#include <unistd.h>

#include <string>
#include <thread>

void callback(std::string domain) {
    printf("%s\n", domain.data());
}

int main() {
    mdns::MDnsSub sub("gialuong", "_http._tcp", "local.", 0);
    sub.ScanDomain(callback);
    while(1) {
        printf("%ld\n", sub.get_domain_list_size());
        if (sub.get_domain_list_size() > 0) {
            printf("at 1: %s\n", sub.get_domain_list_at(1).data());
        }
        sleep(1);
    };
    
    return 0;
}