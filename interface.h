// 212 Клипалко Михаил Михайлович

#ifndef INTERFACE_H
#define INTERFACE_H
#include "controller.h"
class Interface
{
    public:
    double x;
    double y;
    double gx;
    double gy;
    int N;
    Controller controller;
    Interface();
    int Starts();
    ~Interface();
};
#endif
