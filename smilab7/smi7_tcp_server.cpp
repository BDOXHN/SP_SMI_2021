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
    int                 socket_serv,    //descriptor for server socket
                        socket_serv_acc,//descriptor for accept() return socket
                        num_port,       //number of port
                        bind_status,    //status of bind
                        listen_status,  //status of listen
                        data,           //messages from client to server
                        conf;
    struct sockaddr_in  addr_serv,      //struct for description of serv socket address
                        addr_clien;     //struct for description of clien socket address
    socklen_t           len_client;     //dlinna adresa clienta os nezavisimaya
    char                buf[128];       //buf of data with size 128

    /* port input processing */
    if (argc < 2) error("\nYou didnt insert port");
    num_port = atoi(argv[1]);

    /* server socket processing */
    socket_serv = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_serv < 0) error("ERROR creating socket");
    bzero((char*) &addr_serv, sizeof(addr_serv));           //set bytes to 0
    addr_serv.sin_family = AF_INET;
    addr_serv.sin_addr.s_addr = INADDR_ANY;                 //slushaem vse interfaces
    addr_serv.sin_port = htons(num_port);                   //setevoi poryadok byte
    bind_status = bind(socket_serv, (struct sockaddr*) &addr_serv, 
                        sizeof(addr_serv));
    if (bind_status < 0) error("ERROR binding");
    listen_status = listen(socket_serv, 5);
    if(listen_status < 0) error("ERROR listening");

    /* client socket processing */
    len_client = sizeof(addr_clien);

    /* server accept socket processing */
    socket_serv_acc = accept(socket_serv, (struct sockaddr*) &addr_clien, 
                            &len_client);
    if (socket_serv_acc < 0) {
        error("ERROR accepting");
    } else{
        /* get data from client */
        bzero(buf, sizeof(buf));
        data = recv(socket_serv_acc, buf, sizeof(buf), 0);
        if (data < 0) error("ERROR getting");
        printf("\n**getted data: %s", buf);
        printf("\n**i will send it back");
        /* send  confirmtation about getting data*/
        conf = send(socket_serv_acc, buf, sizeof(buf), 0);
        if (conf < 0) error("ERROR sending");
    }
    /* closing sockets */
    close(socket_serv_acc);
    close(socket_serv);
}