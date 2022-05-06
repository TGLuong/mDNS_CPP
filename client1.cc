#include <mdnspub.h>
#include <mdnssub.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CLIENT1_PORT 5050
#define BACKLOG 100


void StartMDNS(mdns::MDnsPub &pub) {
    pub.AddRecordValue("port", "5050");
    pub.Register();
}

int main() {
    int client1_fd;
    struct sockaddr_in client1_addr;

    mdns::MDnsPub pub("client1", "_http._tcp");
    StartMDNS(pub);

    client1_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client1_fd == -1) {
        printf("create socket error\n");
        exit(0);
    }

    memset(&client1_addr, 0, sizeof(client1_addr));
    client1_addr.sin_family = AF_INET;
    client1_addr.sin_port = htons(CLIENT1_PORT); 
    client1_addr.sin_addr.s_addr = INADDR_ANY;

    int opt = 1;
    setsockopt(client1_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(client1_fd, (struct sockaddr *) &client1_addr, sizeof(client1_addr)) == -1) {
        printf("binding error\n");
        exit(0);
    }

    if (listen(client1_fd, BACKLOG) == -1) {
        printf("listen error\n");
        exit(0);
    }

    int new_fd = accept(client1_fd, NULL, NULL);

    uint8_t byte_num;
    read(new_fd, &byte_num, sizeof(byte_num));

    printf("bytenum: %d\n", byte_num);

    while (1) ;

    
    return 0;
}