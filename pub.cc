#include <unistd.h>
#include "mdnspub.h"

#include <iostream>
#include <map>

using namespace std;

int main() {
    mdns::MDnsPub pub("gialuong", "_http._tcp", "local.", 5000, 0);
    pub.Register();
    while (1) { 
        pub.AddRecordValue("age", "21");
        sleep(4);
        pub.AddRecordValue("age", "22");
        sleep(4);
    }
    
    return 0;
}
