#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void error(const char *msg)
{
    perror(msg);
    exit(-1);
}

int main(int argc, char *argv[])
{
    int                 socket_clien,   //descriptor for client cosket
                        num_port,       //number of port
                        connect_status, //status of connect
                        data,           //messages from client to server
                        conf;
    struct sockaddr_in  addr_serv,      //struct for description of serv socket address
                        addr_clien;     //struct for description of client socket address
    socklen_t           len_client;     //dlinna adresa clienta os nezavisimaya
    char                buf[128];       //buf of data with size 128

    /* port input processing */
    if (argc < 2) error("\nYou didnt insert port");
    num_port = atoi(argv[1]);

    /* client socket processing */
    socket_clien = socket(AF_INET, SOCK_DGRAM, 0);          //SOCK_DRGAM
    if (socket_clien < 0) error("ERROR creating socket");
    bzero((char*) &addr_serv, sizeof(addr_serv));           //set bytes to 0
    addr_serv.sin_family = AF_INET;
    addr_serv.sin_addr.s_addr = inet_addr(argv[2]);
    addr_serv.sin_port = htons(num_port);

    /* client socket processing */
    len_client = sizeof(struct sockaddr_in);

    while(1){
        /* send data to server */
        bzero(buf, sizeof(buf));
        fgets(buf, sizeof(buf), stdin);
        data = sendto(socket_clien, buf, sizeof(buf), 0,
            (struct sockaddr*)&addr_serv, len_client);
        if (data < 0) error("ERROR sending");
        printf("\n**sended data: %s", buf);
        printf("\n**i trynna get confirmation of send");
        bzero(buf, sizeof(buf));

        /* get confirmtation about sending data*/
        conf = recvfrom(socket_clien, buf, sizeof(buf), 0,
            (struct sockaddr*)&addr_serv, &len_client);
        if (conf < 0) error("ERROR sending");
        printf("\n**getted data: %s", buf);
        printf("\n**Close connect");
        break;
    }
    /* closing sockets */
    close(socket_clien);
}