/* 
 * File:   unix.h
 * Author: snickers
 *
 * Created on 20 Февраль 2013 г., 19:34
 */

#ifndef UNIX_H
#define	UNIX_H

#include<dirent.h>
#include<unistd.h>
#include<iostream>
#include<stdlib.h>
#include<string>
#include<fstream>


using namespace std;

const int N = 100; //Количество фалов в каталоге

string listAllFiles(const string &dirName);
string lastCon(string files);
string lastChat(string files);
long long int cutNumb(string line);
string f_read(string fname);
int get_fsize(string fileName);

#endif	/* UNIX_H */

