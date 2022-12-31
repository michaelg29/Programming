#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFLEN 512

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Please provide a port.\n");
        return 1;
    }

    int self_sock = 0;
    struct sockaddr_in self_addr;
    short port = (short)atoi(argv[1]);

    struct sockaddr_in other_addr;
    int other_addr_len = sizeof(other_addr);

    char buf[BUFLEN];
    int recvbytes = 0;

    // create socket
    if ((self_sock = socket(AF_INET, SOCK_DGRAM | SOCK_CLOEXEC, IPPROTO_UDP)) < 0)
    {
        printf("socket() failed\n");
        return 1;
    }

    // allocate address
    memset(&self_addr, 0, sizeof(self_addr));
    self_addr.sin_family = AF_INET;
    self_addr.sin_port = htons(port);
    self_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    // bind
    if (bind(self_sock, (struct sockaddr*)&self_addr, sizeof(self_addr)) < 0)
    {
        printf("bind() failed\n");
        return 2;
    }

    printf("Listening on port %d\n", ntohs(self_addr.sin_port));

    // listen for data
    while (1)
    {
        printf("Waiting for data...\n");

        // receive data
        if ((recvbytes = recvfrom(self_sock, buf, BUFLEN, 0, (struct sockaddr *)&other_addr, &other_addr_len)) < 0)
        {
            printf("recvfrom() failed with %d\n", recvbytes);
            //continue;
        }
        else
        {
            buf[recvbytes - 1] = 0;
        }

        printf("Received from %s:%d (%d bytes)\n", inet_ntoa(other_addr.sin_addr), ntohs(other_addr.sin_port), (int)strlen(buf));
        printf("%s\n", buf);

        if (!strcmp(buf, "quit"))
        {
            printf("Exit message received\n");
            break;
        }

        // echo data
        if (recvbytes > 0 && sendto(self_sock, buf, recvbytes, 0, (struct sockaddr*)&other_addr, other_addr_len) != recvbytes)
        {
            printf("sendto() failed\n");
            return 4;
        }
    }

    printf("Exiting\n");
    return 0;
}
