#include <mdnssub.h>
#include <mdnspub.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>


void StartMDNS(mdns::MDnsPub &pub) {
    pub.Register();
}

int main() {
    mdns::MDnsSub sub("client1", "_http._tcp");
    mdns::MDnsPub pub("client2", "_http._tcp");
    StartMDNS(pub);

    std::string ip_address = sub.GetIpAddress();

    printf("ip_address: %s\n", ip_address.data());



    return 0;
}
