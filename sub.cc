#include "mdnssub.h"
#include <stdio.h>
#include <unistd.h>

#include <string>
#include <thread>

void callback(std::string domain) {
    printf("%s\n", domain.data());
}

void OnAddService(std::string service) {
    printf("serive add : %s\n", service.data());
}

void OnRemoveService(std::string service) {
    printf("service remove: %s\n", service.data());
}

int main() {
    mdns::MDnsSub sub("gialuong", "_http._tcp", "local.", 0);
    sub.ScanDomain(callback);
    sub.ScanService(OnAddService, OnRemoveService);
    while(1) {
        printf("domain num: %ld\n", sub.get_domain_vector_size());
        // printf("service num: %ld\n", sub.get_service_vector_size());
        sleep(5);
    };
    
    return 0;
}