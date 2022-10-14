#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "field.h"
#include "saves.h"
class Controller
{
    bool Status_Work;
    public:
        int NCL;
        Controller();
        Field field;
        Saves sv;
        void raise();
        int Print_Field();
        void k_means(int k);
        void Create_Cloud(int N, double x, double y, double gx, double gy);
        int Print_Cloud(int k);
        void find_cluster(int k);
        void Wave();
        void dbscan(int m);
        ~Controller();
};

#endif
