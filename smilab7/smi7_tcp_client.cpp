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
    struct sockaddr_in  addr_serv;      //struct for description of serv socket address
    char                buf[128];       //buf of data with size 128

    /* port input processing */
    if (argc < 2) error("\nYou didnt insert port");
    num_port = atoi(argv[1]);

    /* client socket processing */
    socket_clien = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_clien < 0) error("ERROR creating socket");
    bzero((char*) &addr_serv, sizeof(addr_serv));           //set bytes to 0
    addr_serv.sin_family = AF_INET;
    addr_serv.sin_addr.s_addr = inet_addr(argv[2]);
    addr_serv.sin_port = htons(num_port);

    /* connect with server processing */
    connect_status = connect(socket_clien, (struct sockaddr*) &addr_serv, 
                            sizeof(addr_serv));
    if (connect_status < 0) {
        error("ERROR connecting");
    } else{
        /* send data to server */
        bzero(buf, sizeof(buf));
        fgets(buf, sizeof(buf), stdin);
        data = send(socket_clien, buf, sizeof(buf), 0);
        if (data < 0) error("ERROR sending");
        printf("\n**sended data: %s", buf);
        printf("\n**i trynna get confirmation of send");
        bzero(buf, sizeof(buf));
        /* get confirmtation about sending data*/
        conf = recv(socket_clien, buf, sizeof(buf), 0);
        if (conf < 0) error("ERROR sending");
        printf("\n**getted data: %s", buf);
        printf("\n**Close connect");
    }
    /* closing sockets */
    close(socket_clien);
}