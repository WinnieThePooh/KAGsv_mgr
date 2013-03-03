/* 
 * File:   mgr.h
 * Author: snickers
 *
 * Created on 3 Март 2013 г., 12:48
 */

#ifndef MGR_H
#define	MGR_H

#include<string>

using namespace std;

class mgr {
public:
    mgr();
    mgr(const mgr& orig);
    virtual ~mgr();

    string cut_lnick(string &str, int rpos);
    string cut_rnick(string &str, int rpos);
    bool is_vip(string &player, string &VIP);

private:

};

#endif	/* MGR_H */

