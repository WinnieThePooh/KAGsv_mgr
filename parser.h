/* 
 * File:   parser.h
 * Author: snickers
 *
 * Created on 4 Февраль 2013 г., 14:27
 */
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/stat.h>

#include "unix.h"
#include "telnet.h"
#include "mgr.h"
#include "player.h"


using namespace std;

#ifndef PARSER_H
#define	PARSER_H

class parser {
public:
    parser(string dir);
    parser(const parser& orig);
    virtual ~parser();
private:
    mgr *m_kag;
    telnet *srv;
    player *pl;

    string KAG_DIR, s_mgr, VIP, RCON, MOTD;
    int con_pos, chat_pos, PORT;
    ifstream c_log, chat_log;

    void parse_Logs(string console, string chat);
    bool console_log_parse(const string fname); //Анализируем вывод из консоли
    bool chat_log_parse(const string fname);

    bool parse_console_str(string &str);
    bool get_kills(string &str);
    bool get_chat_commands(string &str);
    
    void motd(int n);
};

#endif	/* PARSER_H */

