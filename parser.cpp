/* 
 * File:   parser.cpp
 * Author: snickers
 * 
 * Created on 4 Февраль 2013 г., 14:27
 */

#include "parser.h"
#include <time.h>

parser::parser(string dir) {
    KAG_DIR = dir;
    RCON = "159357zxc"; //Пароль на сервере, sv_tcpr = 1; - обязательно
    PORT = 50103;
    VIP = "SnIcKeRs; XpeH; SalvaTioN; MrDeath; Toks4700; House_M_D; 16th; Cpa3y; heket123; fantamas2d; Screeam;";
    s_mgr = "<<KAGsv_mgr>> ";

    MOTD = "vk.com/kingarthursgold";

    con_pos = 0;
    chat_pos = 0;


    string console, chat, DIRlogs;

    DIRlogs = listAllFiles(dir + "/Logs/");
    //cout << DIRlogs << endl; //выводим список всех файлов в папке Logs

    if (!DIRlogs.empty()) {
        console = lastCon(DIRlogs);
        chat = lastChat(DIRlogs);

        // cout<< DIRlogs;
        cout << "last console: " << console << endl;
        cout << "last chat: " << chat << endl;

        //Парсим с конца файла, получаем размер фалов
        con_pos = get_fsize(dir + "/Logs/" + console);
        chat_pos = get_fsize(dir + "/Logs/" + chat);

        //Соеденяемся с сервером
        srv = new telnet;
        srv->s_connect("0", PORT); // 0- локальная петля
        sleep(1);
        srv->auth(RCON);

        //парсим вывод
        m_kag = new mgr;
        parse_Logs(dir + "/Logs/" + console,
                dir + "/Logs/" + chat);
    } else {
        cout << "Каталог пустой или не существует!!!" << endl;
        exit(1);
    }


}

parser::parser(const parser& orig) {

}

parser::~parser() {
}

void parser::parse_Logs(string console, string chat) {

    long int time1 = time(NULL);
    int n = 0;
    while (true) {

        /*  if (!console_log_parse(console)) {
              printf("Ошибка получения вывода консоли!");
              break;
          }
          sleep(1);*/

        if (!chat_log_parse(chat)) {
            printf("Ошибка получения вывода чата!");
            break;
        }
        sleep(1);

        long int time2 = time(NULL);
        if (difftime(time2, time1) > 10 * 60) {
            // motd(MOTD);//раз в 10 минут выводить MOTD сообщение
            sleep(1);
            
            motd(n);
            
            if (n!=0) n=0;
            else n = 1;
            
            time1 = time2;
        }
    }
    delete(srv);
    delete(m_kag);
}

//Чтение и анализ логов консоли

bool parser::console_log_parse(const string fname) {
    string old_line, new_line;
    int l_pos;

    c_log.open(fname.c_str());

    if (!c_log.is_open()) return false;

    c_log.seekg(con_pos - 1); //Ставим курсор на новую позицию в файле, -1 чтобы файл заного не грузился

    if (c_log.tellg() == -1) return false;

    while (c_log)//пока возможен вывод
    {
        old_line = new_line;

        getline(c_log, new_line); //получаем строку из файла

        /////////////////////////////////////////////////////////////
        if (new_line != old_line && new_line != "") {
            cout << new_line << endl; //Выводим строку, если она не пустая

            //Если находим совпадение, то останавливаем проверку
            while (true) {
                if (parse_console_str(new_line)) break;
                break; //В любом случае завершаем цикл
            }
        }
        /////////////////////////////////////////////////////////////
        if (l_pos != -1) con_pos = l_pos; //Записываем положения курсора
        l_pos = c_log.tellg();

    }
    c_log.close(); //закрываем файл
    return true;
}

//Чтение и анализ логов чата

bool parser::chat_log_parse(const string fname) {
    string old_line, new_line;
    int l_pos;

    chat_log.open(fname.c_str());

    if (!chat_log.is_open()) return false;

    chat_log.seekg(chat_pos - 1); //Ставим курсор на новую позицию в файле

    if (chat_log.tellg() == -1) return false;

    while (chat_log)//пока возможен вывод
    {
        old_line = new_line;

        getline(chat_log, new_line); //получаем строку из файла

        if (new_line != old_line && new_line != "") {
            //cout << new_line << endl; //Выводим строку, если она не пустая
            /////////////////////////////////////////////////////////////
            //Если находим совпадение, то останавливаем проверку
            while (true) {
                //if (get_kills(new_line)) break;
                if (get_chat_commands(new_line)) break;

                break; //В любом случае завершаем цикл
            }
        }
        /////////////////////////////////////////////////////////////
        if (l_pos != -1) chat_pos = l_pos; //Записываем положения курсора
        l_pos = chat_log.tellg();

    }
    chat_log.close(); //закрываем файл
    return true;
}

bool parser::parse_console_str(string &str) {
    /*
     WARNING: API call failed: cURL Error in putStatus(): Timeout was reached
     WARNING: API call failed: cURL Error in putStatus(): Couldn't resolve host name
     */

    string player;
    int n;

    if (str.find("Closing console device: Signal 2 received") != -1) {
        cout << s_mgr << "!!!Сервер неактивен!!!" << endl;
        return true;
    }

    /*    if (str.find("*Match Ended*")!=-1 || str.find("*Restarting Map*")!=-1)
        {
            cout<<mgr<< "Генерация новой конфигурации карты."<< endl;
            string file= KAG_DIR+"/Base/Maps/mcgen " + KAG_DIR+"/Base/Maps";//как аргумент
            system(file.c_str());
            return true;
        }
     */
    if ((n = str.find(" connected (admin:")) != -1) {
        player = m_kag->cut_lnick(str, n);
        cout << s_mgr << "Игрок " << player << " зашел на сервер" << endl;
        return true;
    }
    return false;
}

bool parser::get_kills(string &str) {
    /*
     * [16:11:27] Henry slew SnIcKeRs with his sword
     * [16:11:24] Adding bot... Henry to team 0
     * [16:11:24] Adding bot... Henry to team 1
     * [16:15:22] [ArSq SnIcKeRs fell on a spike trap
     * [17:05:43] [ArSq SnIcKeRs is now spectating
     * [12:54:00] SnIcKeRs shot Henry with his arrow
     *  Jarl andrija525 gibbed [ArSq SnIcKeRs into pieces

     */

    string player1, player2, player;
    int n, i;

    if ((n = str.find(" slew ")) != -1 && (i = str.find(" with his sword")) != -1) {
        player1 = m_kag->cut_lnick(str, n);
        player2 = m_kag->cut_lnick(str, i);
        cout << "Игрок " << player1 << " зарубил мечом " << player2 << endl;
        return true;
    }

    if ((n = str.find(" fell on a spike")) != -1) {
        player = m_kag->cut_lnick(str, n);
        cout << "Игрок " << player << " упал на шипы" << endl;
        return true;
    }

    return false;
}

bool parser::get_chat_commands(string &str) {
    //Сделать команды только для VIP и выше
    int n;
    string player;

    if ((n = str.find("> /help")) != -1) {
        player = m_kag->cut_lnick(str, n);

        cout << s_mgr << player << " использовал команду /help" << endl;

        if (m_kag->is_vip(player, VIP) == true) {
            cout << s_mgr << player << " может использовать команды." << endl;
            srv->cmd("/msg -------Commands list:-------");
            srv->cmd("/msg help - show all commands");
            srv->cmd("/msg kick [player] - kick player from server");
            srv->cmd("/msg motd - show MOTD message");
            srv->cmd("/msg motd set [message] - set new MOTD message");
            srv->cmd("/msg mpGen - generate new maps");
            srv->cmd("/msg rank [player] - show player's kill/death ratio(don't work)");
            srv->cmd("/msg top - show top 10 players(don't work)");
            srv->cmd("/msg ----------------------------");
        }
        return true;
    }

    if ((n = str.find("> /top")) != -1) {
        player = m_kag->cut_lnick(str, n);

        if (m_kag->is_vip(player, VIP) == true) {
            srv->cmd("/msg -------Top 10 players:-------");
            srv->cmd("/msg 1. Player");
            srv->cmd("/msg 2. Player");
            srv->cmd("/msg 3. Player");
            srv->cmd("/msg 4. Player");
            srv->cmd("/msg 5. Player");
            srv->cmd("/msg 6. Player");
            srv->cmd("/msg 7. Player");
            srv->cmd("/msg 8. Player");
            srv->cmd("/msg 9. Player");
            srv->cmd("/msg 10. Player");
            srv->cmd("/msg -----------------------------");
        }
        return true;
    }

    if ((n = str.find("> /kick")) != -1) {
        //Трололо команда, кикает того кто ее ввел
        player = m_kag->cut_lnick(str, n);

        srv->cmd("/msg " + player + " was kicked.");
        srv->cmd("/msg Reason: Lo-lo-lo Tro-lo-lo :)");
        srv->cmd("/kick " + player);
        return true;
    }

    if ((n = str.find("> /motd set")) != -1) {
        //Установка нового сообщения

        player = m_kag->cut_lnick(str, n);

        if (m_kag->is_vip(player, VIP) == true) {
            MOTD = str.substr(n + 12, str.length() - n); // > /motd set -12 chars
            cout << s_mgr << player << " установил новое сообщение MOTD:" << MOTD << endl;
            srv->cmd("/msg " + player + " set new MOTD.");
        }
        return true;
    }
    
     if ((n = str.find("> /motd")) != -1) {
       
        if (m_kag->is_vip(player, VIP) == true) {
            motd(0);
        }
        return true;
    }

    if ((n = str.find("> /mpGen")) != -1) {
        //Генерация новых карт

        player = m_kag->cut_lnick(str, n);

        if (m_kag->is_vip(player, VIP) == true) {
            srv->cmd("/msg Generating new maps.");
            system("./mcgen 200 ./Base/Maps/ ./Base/Scripts/"); //200 карт
        }
        return true;
    }

    return false;
}

void parser::motd(int n) {
                //КОСТЫЛЬ! Если две одинаковые команды, сообщение не выводится :'(
            if (n == 0) {
                n = 1;
                srv->cmd("/msg " + MOTD);
                srv->cmd("/msg Admins: " + VIP);
            } else {
                n = 0;
                srv->cmd("/msg " + MOTD + ".");
                srv->cmd("/msg Admins: " + VIP);
            }
}

/*
     if (str.find("Closing console device: Signal 2 received")!=-1)
    {
        cout<<mgr<< "!!!Сервер неактивен!!!"<< endl;
        return true;
    }
 */


