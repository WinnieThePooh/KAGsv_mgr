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

string mgr::to_msg(string str) {
    int n,i;
    
    i = str.size();
    
   if((n=str.find("\n\n"))!=-1){
        str.erase(n,i-n);
}
  
    n=1;
    
    while (n > 0) {
        n = str.find("\n",n);
        str.insert(n + 1, "/msg ");
        n++;
        //i--;        
    }
    return str;
}

