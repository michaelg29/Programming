#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define STR_IMPL_(x) #x      //stringify argument
#define STR(x) STR_IMPL_(x)  //indirection to expand argument macros

#define BUFLEN 512
#define BUFLEN_DECREMENT 511
#define SCAN_PROMPT "%" STR(BUFLEN_DECREMENT) "[^\n]"
#define PORT 8889

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Please provide an IP address and port.\n");
        return 1;
    }

    int self_sock = 0;
    struct sockaddr_in self_addr;

    struct sockaddr_in other_addr;
    int other_addr_len = sizeof(other_addr);
    char *other_ip = argv[1];
    short other_port = (short)atoi(argv[2]);

    char buf[BUFLEN];
    int recvbytes = 0;

    // create socket
    if ((self_sock = socket(AF_INET, SOCK_DGRAM | SOCK_CLOEXEC, IPPROTO_UDP)) < 0)
    {
        printf("socket() failed\n");
        return 1;
    }

    // allocate self address
    memset(&self_addr, 0, sizeof(self_addr));
    self_addr.sin_family = AF_INET;
    self_addr.sin_port = htons(PORT);
    self_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // allocate server address
    memset(&other_addr, 0, sizeof(other_addr));
    other_addr.sin_family = AF_INET;
    other_addr.sin_port = htons(other_port);
    other_addr.sin_addr.s_addr = inet_addr(other_ip);

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
        // send data
        scanf(SCAN_PROMPT, buf);
        printf("%s\n", buf);
        continue;
        int n = strlen(buf);
        if (sendto(self_sock, buf, n, 0, (struct sockaddr*)&other_addr, other_addr_len) != n)
        {
            printf("Could not send entire message\n");
            continue;
        }
        else
        {
            printf("Sent %d bytes to %s:%d\n", n, inet_ntoa(other_addr.sin_addr), ntohs(other_addr.sin_port));
        }
        
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
    }

    printf("Exiting\n");
    return 0;
}
