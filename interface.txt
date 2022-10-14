#include "headers.h"
#include <iostream>
#include <string>
#include "interface.h"
#include "controller.h"

// конструктор интерфейса
Interface::Interface()
{
    x = 5.0;            // стандартные значения поля
    y = 5.0;
    gx = 1.0;
    gy = 1.0;
    N = 100;
    Field field();
}

Interface::~Interface(){}

int Interface::Starts()
{
    using namespace std;

    string s1 = "CRCLOUD";
    string s2 = "PRCLOUD";
    string s3 = "PRFIELD";
    string s4 = "WAVE";
    string s5 = "DBSCAN";
    string s6 = "KMEANS";
    string s7 = "RESULT";
    string s8 = "HELP";
    string s9 = "EXIT";

    string s;
    cin >> s;

    if (s == s8)
    {
        string sh;
        fstream fout;
        fout.open("HELP.txt");
        // вывод содержимого
        if (fout.is_open())
        {
            while(!fout.eof())
            {
                getline(fout, sh);
                cout << sh << endl;
            }
        }
        else
        {
            cout << "It seems the file HELP.TXT is not existed\n";
            exit(1);
        }
        return 8;
    }
    if (s == s1) return 1;
    if (s == s2) return 2;
    if (s == s3) return 3;
    if (s == s4) return 4;
    if (s == s5) return 5;
    if (s == s6) return 6;
    if (s == s7) return 7;
    if (s == s9) return 9;



    return 0 ;
}



