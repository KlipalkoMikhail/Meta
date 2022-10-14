#include "headers.h"
#include "saves.h"

// �����, ����������� �������� � ����, ������� ���������: m - ���������� ����� ���������, N - ����� ����� � ����, &points - ������ �� �����
void Saves::cluster_save(int m, int N, vector <Point> &points)
{
    // ������� ������� ������ ���������
    int k = clust[m].size();
    cout << k << endl;
    vector <Cluster> clusters = clust[m];

    // ����� ��� ���
    std::ofstream fout[k];

    // �������� �������� � �����
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

// ����������� Saves
Saves::Saves()
{
    clust.resize(1000);
    codes.resize(1000);
    k = 0;
}

// ��������� �������, ������� ��������� &cl - ��������, ind - ��� ���������
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
