/* 
 * File:   player.h
 * Author: snickers
 *
 * Created on 7 Март 2013 г., 15:19
 */

#ifndef PLAYER_H
#define	PLAYER_H

#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

class player {
public:
    player(string dir);
    player(const player& orig);
    virtual ~player();
    
    string get_stats(const string &player);
    
    void kill(const string &player);
    void die(const string &player);
    void collapse(const string &player);
    
    bool is_vip(const string &player, const string &VIP);
    
private:
    double kills, deaths, collapses; 
    string DIR;
    ofstream f_w;
    ifstream f_r;
    
    void write_stats(const string &player, bool kill, bool death);
    
    bool f_read(const string &player);
    bool f_write(const string &player);
    
   const char* fullname(const string &player);
};

#endif	/* PLAYER_H */

