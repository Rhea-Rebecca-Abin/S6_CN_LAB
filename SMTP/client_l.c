#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXLINE 100
#define BUFSIZE 256 // Increased buffer size to handle longer strings

int main(int argc, char *argv[])
{
    int n;
    int sock_fd;

    struct sockaddr_in servaddr;
    char buf[BUFSIZE]; // Increased buffer size
    char address_buf[MAXLINE], message_buf[MAXLINE];

    if (argc != 3)
    {
        fprintf(stderr, "Command is :./client address port\n");
        exit(1);
    }
    if ((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("Cannot create socket\n");
        exit(1);
    }
    bzero((char *)&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    snprintf(buf, sizeof(buf), "SMTP REQUEST FROM CLIENT\n");
    n = sendto(sock_fd, buf, strlen(buf), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if (n < 0)
    {
        perror("ERROR");
        exit(1);
    }
    if ((n = recvfrom(sock_fd, buf, BUFSIZE, 0, NULL, NULL)) == -1) // Use BUFSIZE
    {
        perror("UDP read error");
        exit(1);
    }
    buf[n] = '\0';
    printf("S:%s", buf);

    snprintf(buf, sizeof(buf), "HELLO name_of_client_mail_server\n");
    n = sendto(sock_fd, buf, strlen(buf), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if ((n = recvfrom(sock_fd, buf, BUFSIZE, 0, NULL, NULL)) == -1) // Use BUFSIZE
    {
        perror("UDP read error");
        exit(1);
    }
    buf[n] = '\0';
    printf("S:%s", buf);

    printf("please enter the email address of the sender: ");
    fgets(address_buf, sizeof(address_buf), stdin);
    address_buf[strlen(address_buf) - 1] = '\0';
    snprintf(buf, sizeof(buf), "MAIL FROM :<%s>\n", address_buf);
    sendto(sock_fd, buf, sizeof(buf), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if ((n = recvfrom(sock_fd, buf, BUFSIZE, 0, NULL, NULL)) == -1) // Use BUFSIZE
    {
        perror("UDP read error");
        exit(1);
    }
    buf[n] = '\0';
    printf("S:%s", buf);

    printf("please enter the email address of the receiver: ");
    fgets(address_buf, sizeof(address_buf), stdin);
    address_buf[strlen(address_buf) - 1] = '\0';
    snprintf(buf, sizeof(buf), "RCPT TO : <%s>\n", address_buf);
    sendto(sock_fd, buf, sizeof(buf), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if ((n = recvfrom(sock_fd, buf, BUFSIZE, 0, NULL, NULL)) == -1) // Use BUFSIZE
    {
        perror("UDP read error");
        exit(1);
    }
    buf[n] = '\0';
    printf("S:%s", buf);

    snprintf(buf, sizeof(buf), "DATA\n");
    sendto(sock_fd, buf, strlen(buf), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if ((n = recvfrom(sock_fd, buf, BUFSIZE, 0, NULL, NULL)) == -1) // Use BUFSIZE
    {
        perror("UDP read error");
        exit(1);
    }
    buf[n] = '\0';
    printf("S:%s", buf);

    do
    {
        fgets(message_buf, sizeof(message_buf), stdin);
        snprintf(buf, sizeof(buf), "%s", message_buf);
        sendto(sock_fd, buf, strlen(buf), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
        message_buf[strlen(message_buf) - 1] = '\0';
        if (strcmp(message_buf, ".") == 0)
            break;
    } while (1);

    if ((n = recvfrom(sock_fd, buf, BUFSIZE, 0, NULL, NULL)) == -1) // Use BUFSIZE
    {
        perror("UDP read error");
        exit(1);
    }
    buf[n] = '\0';
    printf("S:%s", buf);

    snprintf(buf, sizeof(buf), "QUIT\n");
    sendto(sock_fd, buf, strlen(buf), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if ((n = recvfrom(sock_fd, buf, BUFSIZE, 0, NULL, NULL)) == -1) // Use BUFSIZE
    {
        perror("UDP read error");
        exit(1);
    }
    buf[n] = '\0';
    printf("S:%s", buf);

    return 0;
}
/*OUTPUT
gcc client_l.c -o client_l
./client 127.0.0.1 6500
S:220 name_of_server_mail_server
S:250 Hello name_of_client_mail_server
please enter the email address of the sender: sender@example.com
S:250 Hello <sender@example.com>.......sender ok
please enter the email address of the receiver: recv@example.com
S:250 Hello  <recv@example.com>.......Recepient ok
S:354 Enter mail,end with "." on a line by itself 
Hello Dear be positive!!
.
S:250 messages accepted for delivery 
S:221 servers mail server closing connection
*/
