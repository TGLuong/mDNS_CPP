#include "mdnssub.h"
#include <stdio.h>

void callback() {
    printf("haha\n");
}

int main() {
    mdns::MDnsSub sub("gialuong", "_http._tcp", "local.", 0);
    sub.ScanDomain(callback);
    return 0;
}