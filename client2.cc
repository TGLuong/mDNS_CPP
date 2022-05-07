#include <mdnssub.h>
#include <mdnspub.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>


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

void StartMDNS(mdns::MDnsPub &pub, mdns::MDnsSub &sub) {
    pub.Register();
    sub.ScanRecord(OnAddRecord, OnRemoveRecord);
}

int main() {
    mdns::MDnsSub sub("client1", "_http._tcp");
    mdns::MDnsPub pub("client2", "_http._tcp");
    StartMDNS(pub, sub);

    std::string ip_address = sub.GetIpAddress();
    std::string port = sub.get_record_by_key("port");

    printf("ip_address: %s\n", ip_address.data());
    printf("on port: %s\n", port.data());

    return 0;
}
