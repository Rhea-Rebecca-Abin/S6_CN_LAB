#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

void main() {
    int server_sock, acpt_sock, ret_val, i = -1;
    char msg[50] = "Received Frame", read_buff[50], write_buff[50];
    fd_set set;
    struct sockaddr_in serv_addr, other_addr;
    struct timeval timeout;
    socklen_t len;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(7004);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("Binding failed");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    printf("--------Sender of Go back N where N=8\n");
    listen(server_sock, 5);

    len = sizeof(other_addr);
    acpt_sock = accept(server_sock, (struct sockaddr *)&other_addr, &len);
    if (acpt_sock == -1) {
        perror("Accept failed");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    zero:
    i = i + 1;
    memset(write_buff, 0, sizeof(write_buff));
    strcpy(write_buff, msg);
    write_buff[strlen(msg)] = i + '0';
    printf("To Receiver -> Frame %d\n", i);
    write(acpt_sock, write_buff, sizeof(write_buff));

    i = i + 1;
    sleep(1);

    one:
    memset(write_buff, 0, sizeof(write_buff));
    strcpy(write_buff, msg);
    write_buff[strlen(msg)] = i + '0';
    printf("To Receiver -> Frame %d\n", i);
    write(acpt_sock, write_buff, sizeof(write_buff));

    FD_ZERO(&set);
    FD_SET(acpt_sock, &set);
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;

    ret_val = select(acpt_sock + 1, &set, NULL, NULL, &timeout);
    if (ret_val == -1) {
        perror("Error in select");
    } else if (ret_val == 0) {
        printf("---- TIMER TIMEOUT - PACKET %d SENT LOST ----\n", i - 1);
        printf("-- RESTART TIMER GO BACK N RESENDING PACKETS\n");
        i = i - 2;
        goto zero;
    } else {
        read(acpt_sock, read_buff, sizeof(read_buff));
        if ((i == 6) || (i == 4) || (i == 1)) {
            printf("From Receiver <- %s CUMULATIVE ACKNOWLEDGEMENT\n", read_buff);
        } else {
            printf("From Receiver <- %s INDIVIDUAL ACKNOWLEDGEMENT\n", read_buff);
        }
        printf("--Wn\n");
        i++;
        if (i > 7) exit(EXIT_SUCCESS);
        if ((i == 5) || (i == 3)) {
            i--;
            goto zero;
        }
        if ((i < 8) || (i != 5)) goto one;
    }

    close(acpt_sock);
    close(server_sock);
}
/*OUTPUT

--------Sender of Go back N where N=8
To Receiver -> Frame 0
To Receiver -> Frame 1
From Receiver <- ACK2 CUMULATIVE ACKNOWLEDGEMENT
--Wn
To Receiver -> Frame 2
From Receiver <- ACK3 INDIVIDUAL ACKNOWLEDGEMENT
--Wn
To Receiver -> Frame 3
To Receiver -> Frame 4
From Receiver <- ACK5 CUMULATIVE ACKNOWLEDGEMENT
--Wn
To Receiver -> Frame 5
To Receiver -> Frame 6
---- TIMER TIMEOUT - PACKET 5 SENT LOST ----
-- RESTART TIMER GO BACK N RESENDING PACKETS
To Receiver -> Frame 5
To Receiver -> Frame 6
From Receiver <- ACK7 CUMULATIVE ACKNOWLEDGEMENT
--Wn
To Receiver -> Frame 7
From Receiver <- ACK0 INDIVIDUAL ACKNOWLEDGEMENT
*/
