#include "headers.h"
#include <iostream>
#include <fstream>
#include <random>
#include <iomanip>
#include "controller.h"
#include <iterator>
#include <list>
#include <cmath>

double distance(Point x, Point y)
{
    return sqrt((x.getx() - y.getx())*(x.getx()- y.getx()) + (x.gety() - y.gety())*(x.gety() - y.gety()));
}

void Controller::raise()
{
    NCL++;
}
Controller::Controller()
{
    Status_Work = 1;
    NCL = 0;
}

Controller::~Controller(){}

int Controller::Print_Field()
{

    using namespace std;
    if (field.work() != 1)
    {
        cout << "Field is not created\n" << endl;
        return -1;
    }
    else
    {
        cout << " X " << "\t\t\t" << " Y " << endl;
        for (int i = 0; i < field.size(); i++)
        {
            cout << field.getx_p(i) << "\t\t";
            cout << field.gety_p(i) << endl;
        }

    }
    field.file_save();
    return 0;
}

void Controller::Create_Cloud(int N, double x, double y, double gx, double gy)
{
    cin >> N >> x >> y >> gx >> gy;
    field.state_gen(N, x, y, gx, gy, NCL);
    std::cout << "Great!" << endl;
}

int Controller::Print_Cloud(int i)
{
    using namespace std;
    if (((*field.get_cl())[i]).work() != 1)
    {
        cout << "Cloud is not created\n" << endl;
        return -1;
    }
    else
    {
        cout << " X " << "\t\t\t" << " Y " << endl;

        for (int k = 0; k < (*field.get_cl())[i].size(); k++)
        {
            cout << fixed << setprecision(6) << (*field.get_cl())[i].getx_p(k) << "\t\t";
            cout << fixed << setprecision(6) << (*field.get_cl())[i].gety_p(k) << "\t\t" << "\t Col " << (*field.get_cl())[i].getnp_p(k) << endl;
        }
    }
    return 0;
}

// ����� ���������� �� ������ ������������ ���������
void Controller::find_cluster(int k)
{
    // ������� ������ �����
    vector <Point> points = field.p();

    // �������� � ����
    sv.cluster_save(k, field.size(), points);

    cout << "Check the folder. The result are saved." << endl;
}

// �������� ��������, ������� ���������� �� ���������
void Controller::Wave()
{
    // ������� ��������, ������� ����������, ������� �����, ������� a, b, nul, ���������� N - ����� �����, T - ������
    vector <Cluster> clusters;

    int N = field.size();

    vector <vector <double>> D;
    vector <vector <int>> B;

    vector <int> a(N);
    vector <int> b(N);
    vector <int> nul(N);

    int T = 1;

    // �������� ������
    D.resize(N);
    B.resize(N);
    double step = 0;

    // �������� ������
    for (int i = 0; i < N; i++)
    {
        D[i].resize(N);
        B[i].resize(N);
    }

    // ������� ������� ����������
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            D[i][j] = distance(field.p(i), field.p(j));
            step += D[i][j]/2;
        }
    }

    // ������� �����
    step = step/(2*N*(N - 1));

    // ������� ������� �����
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if(D[i][j] < step)
                B[i][j] = 1;
            else B[i][j] = 0;
        }
    }

    // ����������� ������
    D.clear();

    // ������� �����: index - �����, �������� � ������ T, indexnew - ��� ������ ���� �������� �����
    list <int> index;
    list <int> indexnew;
    // ������� �������: temp - ������ ��� ������ ��������, blong - ������ �������� � �� �������� �����
    vector <int> temp(N);
    vector <int> blong(N);

    // �������� �����
    for (int k = 0; k < N; k++)
    {
        if (!blong[k])
        {
            // ������� ������� � ��������� � ���� �����
            Cluster cl;
            cl.reserve(N);
            blong[k] = 1;
            a[k] = 1;
            cl.add_p(k);

            // ������� ����, �������� ����� ���������� �����
            b = a;
            index.push_back(k);

            // �������� �� ����� ��������� �����
            while(1)
            {
                for(list <int> :: iterator it = index.begin(); it != index.end(); it++)
                {
                    for(int i = 0 ; i < N; i++)
                    {
                        // ��������� ��������� �� ����� ������ � ���������� �� ���
                        if(((B[*it][i]) == 1) && (a[i] == 0) && (blong[i] == 0))
                        {
                            // �������� �����
                            b[i] = T + 1;
                            // ��������� � �������
                            cl.add_p(i);
                            // �������, ��� �� ��� ��� ������
                            blong[i] = 1;
                            // ���������� ����� � ���������� ����� (��� ��������� ������)
                            indexnew.push_back(i);
                        }
                    }
                }

                // ����� ����� ������� ���������� ����������� � ������ index
                index.clear();
                index.merge(indexnew);
                indexnew.clear();

                if (a == b) break;
                temp = a;
                a = b;
                T = T + 1;
            }
            clusters.push_back(cl);
        }
        // ��������� ��������� T, a, b
        T = 1;
        a = b = nul;
    }
    // ��������� ���������
    sv.save(clusters, 2);
    cout << "The wave algorithm has done its work\n";
}

// DBscan, m - ����� �������, ����� ����� �������� �����
void Controller::dbscan(int m)
{
    // ������� ��������, ������� ����������, ������� �����, ������� a, b, nul, ���������� N - ����� �����, T - ������
    vector <Cluster> clusters;

    int N = field.size();

    vector <vector <double>> D;
    vector <vector <int>> B;

    vector <int> a(N);
    vector <int> b(N);
    vector <int> nul(N);

    int T = 1;

    // �������� ������
    D.resize(N);
    B.resize(N);
    double step = 0;

    // �������� ������
    for (int i = 0; i < N; i++)
    {
        D[i].resize(N);
        B[i].resize(N);
    }

    // ������� ������� ����������
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            D[i][j] = distance(field.p(i), field.p(j));
            step += D[i][j]/2;
        }
    }

    // ������� �����
    step = step/(2*N*(N - 1));

    // ������� ������� �����
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if(D[i][j] < step)
                B[i][j] = 1;
            else B[i][j] = 0;
        }
    }

    // ����������� ������
    D.clear();

    // ������� �����: index - �����, �������� � ������ T, indexnew - ��� ������ ���� �������� �����
    list <int> index;
    list <int> indexnew;

    // ������� �������: temp - ������ ��� ������ ��������, blong - ������ �������� � �� �������� �����
    vector <int> temp(N);
    vector <int> blong(N);

    // �������� �����
    for (int k = 0; k < N; k++)
    {
        if (!blong[k])
        {
            // ������� ������� � ��������� � ���� �����, ���� ��� �� ����, �� ����������
            if (!iscore(B, k, m)) continue;
            Cluster cl;
            cl.reserve(N);
            blong[k] = 1;
            a[k] = 1;
            cl.add_p(k);

            // ������� ����, �������� ����� ���������� �����
            b = a;
            index.push_back(k);

            // �������� �� ����� ��������� �����
            while(1)
            {
                for(list <int> :: iterator it = index.begin(); it != index.end(); it++)
                {
                    // ���������, �������� �� ����� �����, ���� ��, �� � ������ ������ ��������� �� �������
                    if(!iscore(B, *it, m)) continue;

                    for(int i = 0 ; i < N; i++)
                    {
                        // ��������� ��������� �� ����� ������ � ���������� �� ���
                        if(((B[*it][i]) == 1) && (a[i] == 0) && (blong[i] == 0))
                        {
                            // �������� �����
                            b[i] = T + 1;
                            // ��������� � �������
                            cl.add_p(i);
                            // �������, ��� �� ��� ��� ������
                            blong[i] = 1;
                            // ���������� ����� � ���������� ����� (��� ��������� ������)
                            indexnew.push_back(i);
                        }
                    }
                }

                // ����� ����� ������� ���������� ����������� � ������ index
                index.clear();
                index.merge(indexnew);
                indexnew.clear();

                if (a == b) break;
                temp = a;
                a = b;
                T = T + 1;
            }
            clusters.push_back(cl);
        }
        // ��������� ��������� T, a, b
        T = 1;
        a = nul;
        b = nul;
    }
    // ������� ������� ��� �����, �� �������� � ������ ��������
    Cluster cl;
    cl.reserve(N);

    // ���������� � �������
    for (int i = 0; i < N; i++)
    {
        if (!blong[i]) cl.add_p(i);
    }

    // ������� ���������� � ��������� ������ ���������
    clusters.push_back(cl);
    sv.save(clusters, 3);

    cout << "DBscan has perfectly done\n" << endl;
}

// �������� k_means, ������� ��������� - k - ���������� ���������
void Controller::k_means(int k)
{
    // ��� ��������
    // ����� �����, ����� ���������� � ������ �������
    // ������ �� � ������ ������, ��� ���������� �����������
    // ������ ��� �� ����� �������
    // ������������� ������
    // ���� ������, ���������� ���������� ��������� � ��������

    vector <Point> points = field.p();
    int N = field.size();
    vector <vector <short int>> vec(k);
    vector <Cluster> clusters(k);

    // ������� �������� � ��������� � ��� �� ����� �����
    for (int i = 0; i < k; i++)
    {
        // ������� ������ �� N ���������
        vec[i].resize(N);
        // ������� ������ � �������� �� N ���������
        clusters[i].reserve(N);
        // ��������� �����
        clusters[i].add_p(i);
        // ������� �����
        clusters[i].ccenter(points, N);
    }

    while(1)
    {
        // ������������� ������ � 0

        bool flag = 0;
        for (int i = 0; i < N; i++)
        {
            // ���� ����������� ����������
            int indexmin = 0;
            double min = distance(points[i], *(clusters[0].getcen()));

            // ��������� �������� ���������
            vec[0] = clusters[0].p();

            for (int j = 1; j < k; j++)
            {
                // ��������� �����
                vec[j] = clusters[j].p();
                if (min > distance(points[i], *(clusters[j].getcen())))
                {
                    min = distance(points[i], *(clusters[j].getcen()));
                    indexmin = j;
                }
            }

            // ������� �����, ������� � ������� �� ��������
            for (int j = 0; j < k; j++)
            {
                clusters[j].rem_p(i);
            }

            // ��������� ������ � ��� �������, �������� ��� ����� �����������
            clusters[indexmin].add_p(i);

            // ���� ��������� ����, �� ��������� ������ � 1
            for (int m = 0; m < k; m++)
                if (vec[m] != clusters[m].p()) flag = 1;
        }
        // ������������� ������
        for (int i = 0; i < k; i++)
            clusters[i].ccenter(points, N);

        // ��������� ������
        if (flag == 0) break;
    }
    cout << "K means algorithm has perfectly done" << endl;

    // ��������� ���������
    sv.save(clusters, 1);
}
