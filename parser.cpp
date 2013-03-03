/* 
 * File:   parser.cpp
 * Author: snickers
 * 
 * Created on 4 Февраль 2013 г., 14:27
 */
//Блабла изменения
#include "parser.h"

parser::parser(string dir) {
    KAG_DIR = dir;
    pos = 0;
    chat_pos = 0;
    mgr = "<<KAGsv_mgr>> ";
    VIP = "SnIcKeRs";

    string console, chat, DIRlogs;


    DIRlogs = listAllFiles(dir + "/Logs/");
    cout << DIRlogs << endl;

    if (!DIRlogs.empty()) {
        console = lastCon(DIRlogs);
        chat = lastChat(DIRlogs);

        // cout<< DIRlogs;
        cout << "last console: " << console << endl;
        // cout<<"last chat: "<< chat<< endl;

        //Парсим с конца файла
        pos = get_fsize(dir + "/Logs/" + console);
        chat_pos = get_fsize(dir + "/Logs/" + chat);

        //Соеденяемся с сервером
        srv = new telnet;
        srv->s_connect("0", 50301);
        sleep(1);
        srv->auth("159357zxc");

        //парсим вывод
        parse_Logs(dir + "/Logs/" + console,
                dir + "/Logs/" + chat);
    } else {
        cout << "Каталог пустой или не существует!!!" << endl;
    }


}

parser::parser(const parser& orig) {

}

parser::~parser() {
    delete(srv);
}

void parser::parse_Logs(string console, string chat) {
    while (true) {

        if (!console_log_parse(console)) {
            printf("Ошибка получения вывода консоли!");
            break;
        }

        if (!chat_log_parse(chat)) {
            printf("Ошибка получения вывода чата!");
            break;
        }
        //usleep(1000000);//пауза
        sleep(1);
    }
    delete(srv);
}

//Чтение и анализ логов консоли

bool parser::console_log_parse(const string fname) {
    string old_line, new_line;
    int l_pos;

    c_log.open(fname.c_str());

    if (!c_log.is_open()) return false;

    c_log.seekg(pos); //Ставим курсор на новую позицию в файле

    if (c_log.tellg() == -1) return false;

    while (c_log)//пока возможен вывод
    {
        old_line = new_line;

        getline(c_log, new_line); //получаем строку из файла

        if (new_line != old_line && new_line != "") {
            //cout << new_line << endl;//Выводим строку, если она не пустая

            //Если находим совпадение, то останавливаем проверку
            while (true) {
                if (parse_console_str(new_line)) break;
                break; //В любом случае завершаем цикл
            }
        }
        if (l_pos != -1) pos = l_pos; //Записываем положения курсора
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

    chat_log.seekg(chat_pos); //Ставим курсор на новую позицию в файле

    if (chat_log.tellg() == -1) return false;

    while (chat_log)//пока возможен вывод
    {
        old_line = new_line;

        getline(chat_log, new_line); //получаем строку из файла

        if (new_line != old_line && new_line != "") {
            //cout << new_line << endl;//Выводим строку, если она не пустая

            //Если находим совпадение, то останавливаем проверку
            while (true) {
                if (get_kills(new_line)) break;
                if (get_chat_commands(new_line)) break;

                break; //В любом случае завершаем цикл
            }
        }
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
    int n;
    if (str.find("Closing console device: Signal 2 received") != -1) {
        cout << mgr << "!!!Сервер неактивен!!!" << endl;
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
        string player;
        player = cut_lnick(str, n);
        cout << mgr << "Игрок " << player << " зашел на сервер" << endl;
        return true;
    }
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

    int n, i;

    if ((n = str.find(" slew ")) != -1 && (i = str.find(" with his sword")) != -1) {
        string player1, player2;
        player1 = cut_lnick(str, n);
        player2 = cut_lnick(str, i);
        cout << "Игрок " << player1 << " зарубил мечом " << player2 << endl;

    }

    if ((n = str.find(" fell on a spike")) != -1) {
        string player;
        player = cut_lnick(str, n);
        cout << "Игрок " << player << " упал на шипы" << endl;
        return true;
    }

}

bool parser::get_chat_commands(string &str) {
    //Сделать команды только для VIP и выше
    int n;

    if ((n = str.find("> /help")) != -1) {
        string player;
        player = cut_lnick(str, n);

        cout << mgr << player << " использовал команду /help" << endl;

        if (is_vip(player) == true) {
            cout << mgr << player << " может использовать команды." << endl;
            srv->cmd("/msg -------Commands list:-------");
            srv->cmd("/msg help - show all commands");
            srv->cmd("/msg rank [player] - show plyer's kill/death ratio");
            srv->cmd("/msg top - show top 10 players");
            srv->cmd("/msg ----------------------------");
        }
        return true;
    }

    if ((n = str.find("> /top")) != -1) {
        string player;
        player = cut_lnick(str, n);

        if (is_vip(player) == true) {
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

    }
    return false;
}

/*
     if (str.find("Closing console device: Signal 2 received")!=-1)
    {
        cout<<mgr<< "!!!Сервер неактивен!!!"<< endl;
        return true;
    }
 */

string parser::cut_lnick(string &str, int rpos) {
    int lpos;
    string player;
    lpos = str.rfind(" ", rpos - 1);
    player = str.substr(lpos + 1, rpos - lpos - 1); //*lpos-1 cut space
    return player;
}

string parser::cut_rnick(string &str, int lpos) {
    int rpos;
    string player;
    rpos = str.find(" ", lpos);
    player = str.substr(lpos, rpos - lpos); //*lpos-1 cut space
    return player;
}

int parser::get_fsize(string fileName) {
    ifstream file(fileName.c_str(), ios::binary | ios::ate);
    int size = file.tellg();
    return size;
}

bool parser::is_vip(string player) {
    //может сработать, если явл частью ника!!!! нужно исправить
    if (VIP.find(player) == -1) return false;
    return true;
}
