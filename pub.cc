#include <unistd.h>
#include "mdnspub.h"

#include <iostream>
#include <map>

using namespace std;

int main() {
    mdns::MDnsPub pub("gialuong", "_http._tcp", "local.", 5000, 0);
    pub.Register();
    while (1) {
        pub.AddRecordValue("name", "luong");
        sleep(7);

        std::map<std::string, std::string> record_value = {{"name", "gia luong"}, {"age", "21"}};  
        pub.AddRecordValue(record_value);
        pub.RemoveRecordValue("age");
        sleep(7);
    }
    return 0;
}
