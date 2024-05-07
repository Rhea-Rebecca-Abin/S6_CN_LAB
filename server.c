// Server (FTP)
#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    FILE *fp;
    int sd, newsd, ser, n, a, cli, pid, bd, port, clilen;
    char name[100], fileread[100], fname[100], ch, file[100], rcv[100];
    struct sockaddr_in servaddr, cliaddr;

    // Prompt user to enter the port address
    printf("Enter the port address: ");
    scanf("%d", &port);

    // Create socket
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0)
        printf("Cant create!\n");
    else
        printf("Socket is created.\n");

    // Initialize server address structure
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    a = sizeof(servaddr);

    // Bind socket
    bd = bind(sd, (struct sockaddr *)&servaddr, a);
    if (bd < 0)
        printf("Cant bind!\n");
    else
        printf("Binded.\n");

    // Listen for connections
    listen(sd, 5);

    clilen = sizeof(cliaddr);

    // Accept connection
    newsd = accept(sd, (struct sockaddr *)&cliaddr, &clilen);
    if (newsd < 0) {
        printf("Can't accept!\n");
    } else
        printf("Accepted.\n");

    // Receive file name from client
    n = recv(newsd, rcv, 100, 0);
    rcv[n] = '\0';

    // Open requested file
    fp = fopen(rcv, "r");
    if (fp == NULL) {
        // Send error message if file does not exist
        send(newsd, "error!", 5, 0);
        close(newsd);
    } else {
        // Send file contents to client
        while (fgets(fileread, sizeof(fileread), fp)) {
            if (send(newsd, fileread, sizeof(fileread), 0) < 0) {
                printf("Cannot send file contents!\n");
            }
            sleep(1); // Introducing delay between sends
        }

        // Send completion message when file transfer is done
        if (!fgets(fileread, sizeof(fileread), fp)) {
            // When file pointer reaches end of file, file transfer “completed” message is sent to accepted client connection using newsd, socket file descriptor.
            send(newsd, "completed", 999999999, 0);
        }
        return (0);
    }
}

