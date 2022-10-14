#ifndef SAVES_H
#define SAVES_H
#include "cluster.h"
class Saves
{
private:
    vector <vector <Cluster>> clust;
    int k;
    vector <int> codes;
public:
    Saves();
    void getcode(int k);
    void save(vector <Cluster> &cl, int ind);
    void cluster_save(int m, int N, vector <Point> &points);
    ~Saves();
};

#endif
