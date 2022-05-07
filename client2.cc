#include <mdnssub.h>
#include <mdnspub.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

void StartMDNS(mdns::MDnsPub &pub, mdns::MDnsSub &sub) {
    pub.Register();
    sub.ScanRecord(NULL, NULL);
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
