#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

void main() {
    int cli_sock, i, j, flag = 1;
    char msg1[50] = "ACK", write_buff[50], read_buff[100];
    struct sockaddr_in client;

    cli_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (cli_sock == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&client, 0, sizeof(client));
    client.sin_family = AF_INET;
    client.sin_port = htons(7004);
    client.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(cli_sock, (struct sockaddr *)&client, sizeof(client)) == -1) {
        perror("Connection failed");
        close(cli_sock);
        exit(EXIT_FAILURE);
    }

    printf("\n--------Receiver of Go back N where N=8\n");

    for (i = 0; i < 8; i++) {
        memset(read_buff, 0, sizeof(read_buff));
        memset(write_buff, 0, sizeof(write_buff));

        if (i == 5 && flag == 1) {
            flag = 0;
            read(cli_sock, read_buff, sizeof(read_buff));
        }
        read(cli_sock, read_buff, sizeof(read_buff));
        if (read_buff[strlen(read_buff) - 1] != i + '0') {
            printf("\n----- PACKET FROM SENDER %d LOST - %d RECEIVED OUT OF ORDER - DISCARDING -----\n", i, i + 1);
            i--;
        } else {
            if (i == 0) {
                printf(" -> %s\n", read_buff);
                read(cli_sock, read_buff, sizeof(read_buff));
                i++;
            }
            printf(" -> %s\n", read_buff);
            if ((i == 5) || (i == 3)) {
                read(cli_sock, read_buff, sizeof(read_buff));
                printf(" -> %s\n", read_buff);
                i++;
            }
            if (i == 7) j = 0;
            else j = i + 1;
            if ((i == 6) || (i == 4) || (i == 1)) {
                printf("\n <- ACK %d ----- CUMULATIVE ACKNOWLEDGEMENT\n", j);
            } else {
                printf("\n <- ACK %d ----- INDIVIDUAL ACKNOWLEDGEMENT\n", j);
            }
            printf("\n-----------------------------------------------------\n");
            strcpy(write_buff, msg1);
            write_buff[strlen(msg1)] = j + '0';
            write(cli_sock, write_buff, sizeof(write_buff));
        }
    }

    close(cli_sock);
}

/* OUTPUT

--------Receiver of Go back N where N=8
 -> Received Frame0
 -> Received Frame1

 <- ACK 2 ----- CUMULATIVE ACKNOWLEDGEMENT

-----------------------------------------------------
 -> Received Frame2

 <- ACK 3 ----- INDIVIDUAL ACKNOWLEDGEMENT

-----------------------------------------------------
 -> Received Frame3
 -> Received Frame4

 <- ACK 5 ----- CUMULATIVE ACKNOWLEDGEMENT

-----------------------------------------------------

----- PACKET FROM SENDER 5 LOST - 6 RECEIVED OUT OF ORDER - DISCARDING -----
 -> Received Frame5
 -> Received Frame6

 <- ACK 7 ----- CUMULATIVE ACKNOWLEDGEMENT

-----------------------------------------------------
 -> Received Frame7

 <- ACK 0 ----- INDIVIDUAL ACKNOWLEDGEMENT
*/
