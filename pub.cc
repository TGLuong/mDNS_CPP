#include <unistd.h>
#include "mdnspub.h"

#include <iostream>
#include <map>

using namespace std;

int main() {
    int i_status;
    mdns::MDnsPub pub("gialuong", "_http._tcp", "local.", 5000, 0);

    printf("register status: %d\n", pub.Register());
    while (1) { 
        i_status = pub.AddRecordValue("age", "21");
        printf("status: %d\n", i_status);
        sleep(4);
        i_status = pub.AddRecordValue("age", "22");
        printf("status: %d\n", i_status);
        sleep(4);
    }
    
    return 0;
}
