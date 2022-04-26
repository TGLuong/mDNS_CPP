#include "mdnspub.h"
#include <iostream>

using namespace std;

int main() {
    mdns::MDnsPub pub(0, "gialuong", "_http._tcp", "local.", "", 5000, 0);
    pub.InitRecord();
    pub.Regist();
    while (1) {
        string key, value;
        cout << "key: ";
        getline(cin, key);
        if (key.compare("exit") == 0) {
            break;
        }
        cout << "value: ";
        getline(cin, value);

        pub.AddRecordValue(key, value);
        pub.UpdateRecord();
    }
    pub.DestroyRecord();
    pub.Unregist();
    return 0;
}