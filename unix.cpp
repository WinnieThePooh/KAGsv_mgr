/* 
 * File:   unix.cpp
 * Author: snickers
 * 
 * Created on 20 Февраль 2013 г., 19:34
 */

#include "unix.h"

string listAllFiles(const string &dirName) {
    DIR *dirp = opendir(dirName.c_str());
    string files;

    if (dirp) {
        struct dirent *dp = NULL;

        while ((dp = readdir(dirp)) != NULL) {
            string file(dp->d_name);

            if (file == "." || file == "..") // skip these
                continue;

            //cout <<file << endl;
            files.append(file + "\n");

            // }
        }

        closedir(dirp);
    }

    return files;
}

string lastLog(string files, string prefix) {
    string arr[100];
    string last;

    int n, i, l;
    n = 0;
    i = 0;
    l = 0;

    while (true) {
        if ((n = files.find(prefix, n)) != -1) {
            i = files.find(".txt", n) + 4;
            arr[l] = files.substr(n, i - n);

            n = i;
            l++;
        } else {
            l--;
            break;
        }
    }

    last = arr[l];
    while (l > 0) {

        if (cutNumb(last) < cutNumb(arr[l])) {
            last = arr[l];
        }

        l--;
    }

    return last;
}

long long int cutNumb(string line) {//from 13-02-20-16-29-23 to 130220162923
    int n;
    long long int numb;


    if (line.find("console-") != -1) {
        line.erase(0, 8);
    }
    if (line.find("chat-") != -1) {
        line.erase(0, 5);
    }
    if (n = line.find(".txt")) {
        line.erase(n, line.length() - n);
    }

    n = 0;

    while (true) {
        if ((n = line.find("-", n)) != -1) {
            line.erase(n, 1);
        } else {
            numb = atof(line.c_str());
            return numb;
        }
    }
    return 0;
}


string motd_load(string fname){
  string line,str;
  ifstream file (fname.c_str());
  if (file.is_open())
  {
    while ( file.good() )
    {
      getline (file,line);
      str+= line + "\n";
    }
    file.close();
    return str;
  }
  cout<< "err: motd_load()"<<endl;
  return " ";
}

int get_fsize(string fileName) {
    ifstream file(fileName.c_str(), ios::binary | ios::ate);
    int size = file.tellg();
    return size;
}