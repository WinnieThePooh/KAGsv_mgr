/* 
 * File:   player.cpp
 * Author: snickers
 * 
 * Created on 7 Март 2013 г., 15:19
 */

#include "player.h"

player::player(string dir) {
    DIR = dir;

    kills=deaths=0;
}

player::player(const player& orig) {
}

player::~player() {

}

void player::kill(string player) {

    write_stats(player, true, false);
}

void player::die(string player) {

    write_stats(player, false, true);
}

void player::collapse(string player) {

}

bool player::f_read() {
    if (f_r.is_open()) {
        f_r >> kills;
        f_r >> deaths;
        return true;
    }
    return false;
}

bool player::f_write() {
    f_w.seekp(0);
    f_w << kills << " " << deaths;
    return true;
}

void player::write_stats(string &player, bool kill, bool death) {
    string fullname = DIR + player;

    f_r.open(fullname.c_str());
    if (f_read()) {

        if (kill) {
            kills++;
        }

        if (death) {
            deaths++;
        }

        f_r.close();
        
        f_w.open(fullname.c_str());
        f_write();
        f_w.close();
    } else 
    {
        printf("Can't open file %s \n",fullname.c_str());
        
        f_w.open(fullname.c_str());
        f_write();
        f_w.close();
        
    }
    f_r.close();

}

bool player::is_vip(string player, string VIP) {
    //может сработать, если явл частью ника!!!! нужно исправить
    if (VIP.find(player) == -1) return false;
    return true;
}

string player::get_stats(string player)
{
    string fullname = DIR + player;
    f_r.open(fullname.c_str());
    if (f_read()) {
        
        stringstream out;//Преобразование int в string
        out <<"Kills: "<<kills<<" Deaths: "<<deaths<<" k/d: "<<double(kills/deaths);
        
        f_r.close();
        return out.str();
    }
    else printf("Can't open file %s", fullname.c_str());
    
    return "";
}