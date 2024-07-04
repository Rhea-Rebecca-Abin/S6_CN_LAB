#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

void main() {
    int client, y, x, k = 5, m = 1, p;
    char buffer[1024];
    struct sockaddr_in servAddr;
    socklen_t addrSize;

    client = socket(PF_INET, SOCK_STREAM, 0);
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(5600);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(servAddr.sin_zero, '\0', sizeof servAddr.sin_zero);

    y = connect(client, (struct sockaddr*)&servAddr, sizeof servAddr);
    if (y == -1) {
        printf("Error in connection\n");
        exit(1);
    }

    while (k != 0) {
        if (m <= 5)
            printf("Sending %d\n", m);
        
        if (m % 2 == 0)
            strcpy(buffer, "frame");
        else {
            strcpy(buffer, "error");
            printf("Packet loss\n");
            for (p = 1; p <= 3; p++) {
                printf("Waiting for %d seconds\n", p);
                sleep(1);
            }
            printf("Retransmitting ...\n");
            strcpy(buffer, "frame");
        }

        x = send(client, buffer, 19, 0);
        if (x == -1) {
            printf("Error in sending\n");
            exit(1);
        } else
            printf("Sent %d\n", m);

        int z = recv(client, buffer, 1024, 0);
        if (z == -1) {
            printf("Error in receiving data\n");
            exit(1);
        }
        
        k--;
        m++;
        
        if ((strncmp(buffer, "ack", 3) == 0))
            printf("Acknowledgement received for %d...\n", m - 1);
        else
            printf("Ack not received\n");
    }
    close(client);
}
/*OUTPUT
Sending 1
Packet loss
Waiting for 1 seconds
Waiting for 2 seconds
Waiting for 3 seconds
Retransmitting ...
Sent 1
Acknowledgement received for 1...
Sending 2
Sent 2
Acknowledgement received for 2...
Sending 3
Packet loss
Waiting for 1 seconds
Waiting for 2 seconds
Waiting for 3 seconds
Retransmitting ...
Sent 3
Acknowledgement received for 3...
Sending 4
Sent 4
Acknowledgement received for 4...
Sending 5
Packet loss
Waiting for 1 seconds
Waiting for 2 seconds
Waiting for 3 seconds
Retransmitting ...
Sent 5
Acknowledgement received for 5...
*/
