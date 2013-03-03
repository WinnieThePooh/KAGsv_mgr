/* 
 * File:   telnet.cpp
 * Author: snickers
 * 
 * Created on 2 Март 2013 г., 11:34
 */

#include "telnet.h"

telnet::telnet() {

}

telnet::telnet(const telnet& orig) {
}

telnet::~telnet() {
}

void telnet::s_connect(string ip, int port) {
    //Соеденяемся с сервером   
    struct sockaddr_in addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port); // или любой другой порт...

    if (ip != "0") addr.sin_addr.s_addr = inet_addr(ip.c_str());
    else addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); //локальная петля, если не указан адрес

    if (connect(sock, (struct sockaddr *) &addr, sizeof (addr)) < 0) {
        perror("connect");
        exit(2);
    }

}

void telnet::auth(string pass) {
    pass.append("\n");
    send(sock, pass.c_str(), pass.length(), 0);
}

void telnet::close_conn() {
    close(sock);
}

void telnet::cmd(string command) {
    command.append("\n");
    send(sock, command.c_str(), command.length(), 0);
}
