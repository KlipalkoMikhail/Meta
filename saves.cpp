#include "headers.h"
#include "saves.h"

// метод, сохран€ющий кластеры в файл, входные параметры: m - пор€дковый номер алгоритма, N - число точек в поле, &points - ссылка на точки
void Saves::cluster_save(int m, int N, vector <Point> &points)
{
    // создаем удобный вектор кластеров
    int k = clust[m].size();
    cout << k << endl;
    vector <Cluster> clusters = clust[m];

    // файлы под них
    std::ofstream fout[k];

    // печатаем кластеры в файлы
    for (int i = 0; i < k; i++)
    {
        char filename[120];
        sprintf(filename, "%d.dat", i);
        fout[i].open(filename);
        for (int j = 0; j < N; j++)
        {
            if (clusters[i].tr(j) == 1)
                fout[i] << points[j].getx() << "\t\t" << points[j].gety() << endl;
        }
    }
}

// конструктор Saves
Saves::Saves()
{
    clust.resize(1000);
    codes.resize(1000);
    k = 0;
}

// сохран€ем кластер, входные параметры &cl - кластеры, ind - код алгоритма
void Saves::save(vector <Cluster> &cl, int ind)
{
    clust[k] = cl;
    codes[k] = ind;
    k++;
}

Saves::~Saves(){}

void Saves::getcode(int k)
{
    if (codes[k] == 1) cout << "It is K means algorithm\n" << "It was done as " << k << "th algorithm\n";
    if (codes[k] == 2) cout << "It is Wave algorithm\n" << "It was done as " << k << "th algorithm\n";
    if (codes[k] == 3) cout << "It is DBscan algorithm\n" << "It was done as " << k << "th algorithm\n";
}
