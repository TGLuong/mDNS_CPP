#include <mdnssub.h>
#include <mdnssub.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>


void StartMDNS(mdns::MDnsPub &pub) {
    pub.Register();
}

int main() {

    mdns::MDnsPub pub("client2", "_http._tcp", "local.", 5050, 0);
    StartMDNS(pub);



    return 0;
}
