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

void OnAddRecord(std::map<std::string, std::string> service) {
    printf("on add\n");
    std::map<std::string, std::string>::iterator itr;
    for (itr = service.begin(); itr != service.end(); itr++) {
        printf("%s: %s\n", itr->first.data(), itr->second.data());
    }
}

void OnRemoveRecord(std::map<std::string, std::string> service) {
    printf("on remove\n");
    std::map<std::string, std::string>::iterator itr;
    for (itr = service.begin(); itr != service.end(); itr++) {
        printf("%s: %s\n", itr->first.data(), itr->second.data());
    }
}

int main() {
    mdns::MDnsSub sub("gialuong", "_http._tcp", "local.", 0);
    sub.ScanDomain(callback);
    sub.ScanService(OnAddService, OnRemoveService);
    sub.ScanRecord(OnAddRecord, OnRemoveRecord);
    while(1) {
        printf("domain num: %ld\n", sub.get_domain_list_size());
        printf("service num: %ld\n", sub.get_service_list_size());
        printf("record num: %ld\n", sub.get_record_map_size());
        printf("record name: %s\n", sub.get_record_by_key("name").data());
        sleep(2);
    };
    
    return 0;
}