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


using namespace std;

#ifndef PARSER_H
#define	PARSER_H

class parser {
public:
    parser(string dir);
    parser(const parser& orig);
    virtual ~parser();
private:
    telnet *srv;
    string KAG_DIR,mgr, VIP;
    int pos, chat_pos;
    ifstream c_log, chat_log;
    
    void parse_Logs(string console, string chat);
    bool console_log_parse(const string fname);//Анализируем вывод из консоли
    bool chat_log_parse(const string fname);
    
    bool parse_console_str(string &str);
    bool get_kills(string &str);
    bool get_chat_commands(string &str);
    
    string cut_lnick(string &str,int rpos);
    string cut_rnick(string &str,int rpos);

    int get_fsize(string fileName);
    
    bool is_vip(string player);
};

#endif	/* PARSER_H */

