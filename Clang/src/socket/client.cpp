#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

void error_message(int line)
{
    printf("error: %d\n", line);
    exit(1);
}

int main()
{
    int port = 7070;
    char *mes = "hello server";
    char *ip = "127.0.0.1";
    int len = strlen(mes);
    int sock;
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        error_message(__LINE__);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);

    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        error_message(__LINE__);

    printf("RECEIVE: ");
    int total = 0;
    int num;
    char buf[50];

    while (total < len)
    {
        if ((num = recv(sock, buf, 49, 0)) <= 0)
            error_message(__LINE__);

        total += num;
        buf[num] = '\0';
        printf("%s", buf);
    }

    if (send(sock, mes, len, 0) != len)
        error_message(__LINE__);

    printf("\n");
    close(sock);
    exit(1);
    return 0;
}