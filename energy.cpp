#include <fstream>
#include <iostream>
#include <conio.h>
#include <vector>
#include <math.h>
#include <cmath>
#include <map>

using namespace std;

class reader
{
    double px, py, pz, e;

public:
    vector<vector<double>> readDatFile(string s)
    {
        vector<vector<double>> a;

        ifstream fin;
        fin.open(s);
        if (!fin.is_open())
        {
            cout << "NOT ABE TO READ FILE! Press any key to exit";
            getch();
            exit(0);
        }

        // while (!fin.eof())
        // {
        //     fin >> px;
        //     fin >> py;
        //     fin >> pz;
        //     fin >> e;
        //     vector<double> temp;
        //     temp.push_back(px);
        //     temp.push_back(py);
        //     temp.push_back(pz);
        //     temp.push_back(e);
        //     a.push_back(temp);
        // }
        double x;
        int c = 1;
        vector<double> temp;
        while (fin >> x)
        {
            temp.push_back(x);
            if ((c % 4) == 0)
            {
                a.push_back(temp);
                temp.erase(temp.begin(), temp.end());
            }
            c++;
        }
        return a;
    }
};

class data
{
public:
    // double x, y, z;
    // data(double x, double y, double z)
    // {
    //     x = this->x;
    //     y = this->y;
    //     z = this->z;
    // }
    void traverseAxis(double x, double y, double z)
    {
    }
    void anglebwTwoAxis(double x, double y, double z)
    {
    }
};

class FourVector : public data
{
    // double e;

public:
    // FourVector(double x, double y, double z, double e1) : data(x, y, z)
    // {
    //     e = e1;
    // }
    double calcMass(double x, double y, double z, double e)
    {
        double p = pow(x, 2) + pow(y, 2) + pow(z, 2); // p^2
        double mass = sqrt(pow(e, 2) - p);            // m= sqrt(E^2 - P^2)  in form of MeV/c^2
        return mass;
    }
};

void intilizeMap(double minx, double maxx, int bins, map<double, double> &mp)
{
    double width = (maxx - minx) / bins;
    for (double i = minx; i < maxx; i += width)
    {
        mp.insert({i, 0});
    }
}

void updateOutputFile(map<double, double> mp,double maxx)
{
    ofstream fout;
    fout.open("histogram.txt", ios_base::out);
    for (auto it : mp)
    {
        fout << it.first << " " << it.second;
        fout << endl;
    }
    fout << maxx;
}

int main()
{
#pragma region inputbyUser
    // input by user
    string ipFileLoc;
    cout << "Enter Input File Location :-  ";
    cin >> ipFileLoc;
    double minx, maxx, noOfBins;
    cout << "\nEnter minimum value of x :- ";
    cin >> minx;
    cout << "\nEnter maximum value of x :- ";
    cin >> maxx;
    cout << "\nEnter number of bins :- ";
    cin >> noOfBins;
    //
#pragma endregion

#pragma region selectingDistribution
    // selecting distribution
    cout << "\n 1 - Transverse Axis Distribution";
    cout << "\n 2 - X Distribution";
    cout << "\n 3 - Y Distribution";
    cout << "\n 4 - Z Distribution";
    cout << "\n 5 - Energy Distribution";
    cout << "\n 6 - Mass Distribution";
    cout << "\n\nEnter number to select :-  ";
    int choice;
    cin >> choice;
    //
#pragma endregion

#pragma region read data file and map intialization
    // read data file
    reader r;
    FourVector f;
    vector<vector<double>> vec = r.readDatFile(ipFileLoc);

    // map to store histogram
    map<double, double> mp;
    intilizeMap(minx, maxx, noOfBins, mp);
#pragma endregion

#pragma region SwitchCase
    // different distributions
    switch (choice)
    {
    case 1: // Tranverse distribution
    {
        
    }
    break;
    case 2: // pX distribution
    {
        for (int i = 0; i < vec.size(); i++)
        {
            if (vec[i][0] >= minx && vec[i][0] < maxx)
                if (mp.lower_bound(vec[i][0]) == mp.find(vec[i][0]))
                {
                    mp[vec[i][0]]++;
                }
                else
                {
                    auto it = mp.lower_bound(vec[i][0]);
                    it--;
                    (*it).second++;
                }
        }
        updateOutputFile(mp,maxx);
    }
    break;
    case 3:
    {
        for (int i = 0; i < vec.size(); i++)
        {
            if (vec[i][1] >= minx && vec[i][1] < maxx)
                if (mp.lower_bound(vec[i][1]) == mp.find(vec[i][1]))
                {
                    mp[vec[i][1]]++;
                }
                else
                {
                    auto it = mp.lower_bound(vec[i][1]);
                    it--;
                    (*it).second++;
                }
        }
        updateOutputFile(mp,maxx);
    }
    break;
    case 4:
    {
        for (int i = 0; i < vec.size(); i++)
        {
            if (vec[i][2] >= minx && vec[i][2] < maxx)
                if (mp.lower_bound(vec[i][2]) == mp.find(vec[i][2]))
                {
                    mp[vec[i][2]]++;
                }
                else
                {
                    auto it = mp.lower_bound(vec[i][2]);
                    it--;
                    (*it).second++;
                }
        }
        updateOutputFile(mp,maxx);
    }
    break;
    case 5:
    {
        for (int i = 0; i < vec.size(); i++)
        {
            if (vec[i][3] >= minx && vec[i][3] < maxx)
                if (mp.lower_bound(vec[i][3]) == mp.find(vec[i][3]))
                {
                    mp[vec[i][3]]++;
                }
                else
                {
                    auto it = mp.lower_bound(vec[i][3]);
                    it--;
                    (*it).second++;
                }
        }
        updateOutputFile(mp,maxx);
    }
    break;
    case 6:
    {
        for (int i = 0; i < vec.size(); i++)
        {
            double m = f.calcMass(vec[i][0], vec[i][1], vec[i][2], vec[i][3]);
            if (m >= minx && m < maxx)
                if (mp.lower_bound(m) == mp.find(m))
                {
                    mp[m]++;
                }
                else
                {
                    auto it = mp.lower_bound(m);
                    it--;
                    (*it).second++;
                }
        }
        updateOutputFile(mp,maxx);
    }
    break;

    default:
        break;
    }
#pragma endregion
}