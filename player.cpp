/*
 * File: player.cpp
 * Author: snickers
 *
 * Created on 7 Март 2013 г., 15:19
 */

#include "player.h"

player::player(string dir) {
    DIR = dir;

    kills = deaths = 0;
}

player::player(const player& orig) {
}

player::~player() {

}

void player::kill(const string &player) {

    write_stats(player, true, false);
}

void player::die(const string &player) {

    write_stats(player, false, true);
}

void player::collapse(const string &player) {

}

bool player::f_read(const string &player) {
    //если ошибка открытия поворяем действие один раз
    kills = deaths = 0; //обнуляем чтобы при ошибке не выводилась чужая стата

    bool err = false;
    for (int i = 0; i < 1; i++) {

        try {

            f_r.open(fullname(player));
            if (!f_r.is_open()) return false;
            f_r >> kills;
            f_r >> deaths;
            f_r.close();
            f_r.clear();

        } catch (...) {
            cout << "Error! Can't read file" << player << endl;
            err = true;
        }
        if (err == false) return true;
    }
    return false;

}

bool player::f_write(const string &player) {
    bool err = false;
    //если ошибка, то повторяем 1 раз
    for (int i = 0; i < 1; i++) {

        try {
            f_w.open(fullname(player));
            if (!f_w.is_open()) return false;
            f_w.seekp(0);
            f_w << kills << " " << deaths;
            f_w.close();
            f_w.clear();

        } catch (...) {
            cout << "Error! Can't write to file " << player << endl;
            err = true;
        }
        if (err == false) return true;
    }
    return false;
}

void player::write_stats(const string &player, bool kill, bool death) {

    if (f_read(player)) {

        if (kill) {
            kills++;
        }
        if (death) {
            deaths++;
        }
        f_write(player);
        
    } else {
        //При первом выполнении ф-ции, файла может не быть
        //Создаем его

        //Если файл удается создать
        if (f_write(player)) {
            write_stats(player, kill, death);
        } else printf("Can't open/create file\n");
    }
    //f_r.close();

}

bool player::is_vip(const string &player, const string &VIP) {
    //может сработать, если явл частью ника!!!! нужно исправить
    if (VIP.find(player) == -1) return false;
    return true;
}

string player::get_stats(const string &player) {
    if (f_read(player)) {
        stringstream out; //Преобразование в string
        out << "Kills: " << kills << " Deaths: " << deaths << " k/d: " << float(kills / deaths);
        return out.str();
    } else printf("Can't open file");

    return "";
}

const char* player::fullname(const string &player) {
    return string(DIR + player).c_str();
}