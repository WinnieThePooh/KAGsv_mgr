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
    
    string get_stats(string player);
    
    void kill(string player);
    void die(string player);
    void collapse(string player);
    
    bool is_vip(string player, string VIP);
    
private:
    double kills, deaths, collapses; 
    string DIR;
    ofstream f_w;
    ifstream f_r;
    
    void write_stats(string &player, bool kill, bool death);
    
    bool f_read();
    bool f_write();
};

#endif	/* PLAYER_H */

