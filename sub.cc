#include "mdnssub.h"
#include <stdio.h>
#include <unistd.h>

#include <string>
#include <thread>

void callback(std::string domain) {
    printf("local add: %s\n", domain.data());
}

void OnAddService(std::string service) {
    printf("service add: %s\n", service.data());
}

void OnRemoveService(std::string service) {
    printf("service remove: %s\n", service.data());
}

void OnAddRecord(std::string service) {
    printf("service add: %s\n", service.data());
}

void OnRemoveRecord(std::string service) {
    printf("service remove: %s\n", service.data());
}

int main() {
    mdns::MDnsSub sub("gialuong", "_http._tcp", "local.", 0);
    sub.ScanDomain(callback);
    sub.ScanService(OnAddService, OnRemoveService);
    sub.ScanRecord(OnAddRecord, OnRemoveRecord);
    while(1) {
        // printf("domain num: %ld\n", sub.get_domain_list_size());
        // printf("domain 1: %s\n", sub.get_domain_list_at(0).data());
        // printf("service num: %ld\n", sub.get_service_list_size());
        sleep(2);
    };
    
    return 0;
}