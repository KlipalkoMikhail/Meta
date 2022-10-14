#ifndef FIELD_H
#define FIELD_H
#include <vector>
#include "cloud.h"
using namespace std;
class Field
{
    private:
    bool state_work;
    int N;
    int NCL;
    vector <Cloud> clouds;
    vector <Point> points;
    public:

    Field();
    int file_save();
    int size();
    int nclouds();
    bool work();
    vector <Cloud> *get_cl();
    double getx_p(int k);
    double gety_p(int k);
    Point p(int k);
    vector <Point> p();
    void state_gen(int N, double x, double y, double gx, double gy, int i);
};
#endif
