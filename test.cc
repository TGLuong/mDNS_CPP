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
    DNSServiceRef ref;
    int status;

    char mes[10] = {0x1, 0x20};

    status = DNSServiceRegister(&ref, 0, 0, "gialuong", "_http._tcp", "local.", NULL, 5000, 1, NULL, NULL, NULL);

    while (1) {
        status = DNSServiceUpdateRecord(ref, NULL, 0, 2, mes, 120);
        printf("status %d\n", status);
        sleep(1);
    }

    printf("status %d\n", status);
    return 0;
}
