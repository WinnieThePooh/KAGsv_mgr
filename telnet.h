/* 
 * File:   telnet.h
 * Author: snickers
 *
 * Created on 2 Март 2013 г., 11:34
 */

#ifndef TELNET_H
#define	TELNET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string>
#include <cstdlib>

using namespace std;

class telnet {
public:
    telnet();
    telnet(const telnet& orig);
    virtual ~telnet();

    void s_connect(string ip, int port);
    void auth(string pass);
    void close_conn(); //закрытие соеденения
    void cmd(string command);

private:
    int sock;

};

#endif	/* TELNET_H */

