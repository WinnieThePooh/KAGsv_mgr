/* 
 * File:   mgr.cpp
 * Author: snickers
 * 
 * Created on 3 Март 2013 г., 12:48
 */

#include "mgr.h"

mgr::mgr() {
}

mgr::mgr(const mgr& orig) {
}

mgr::~mgr() {
}

string mgr::cut_lnick(string str, int rpos) {
    int lpos;
    string player;
    lpos = str.rfind(" ", rpos - 1);
    player = str.substr(lpos + 1, rpos - lpos - 1); //*lpos-1 cut space
    return player;
}

string mgr::cut_rnick(string str, int lpos) {
    int rpos;
    string player;
    rpos = str.find(" ", lpos);
    player = str.substr(lpos, rpos - lpos); //*lpos-1 cut space
    return player;
}

bool mgr::is_vip(string player, string VIP) {
    //может сработать, если явл частью ника!!!! нужно исправить
    if (VIP.find(player) == -1) return false;
    return true;
}

