// to generate a 1D histogram from a given four vector data file

// header files
#include <fstream> //for file handling
#include <iostream>
#include <conio.h> //for getch()
#include <vector>  //for vector
#include <cmath>
#include <map> //for map
#include <algorithm>

using namespace std;

// This code defines a class called reader, which has a member function called readDatFile
// The purpose of this function is to read data from a file and return it in the form of a vector of vectors

// reader class with readDatfile
class reader
{

private:
    vector<vector<double>> f_vector_data; // create a vector of vectors to store the data

public:
    // member function that reads data from a file and returns it as a vector of vectors
    vector<vector<double>> readDatFile(string s)
    {

        ifstream fin;       // create an input file stream object
        fin.open(s);        // open the file with the name given in the parameter string
        if (!fin.is_open()) // check if the file was opened successfully
        {
            cout << "\nNOT ABLE TO READ FILE! Press any key to exit";
            getch();
            exit(0); // if the file was not opened successfully, print an error message and exit the program
        }

        double x;            // temporary variable to store each value read from the file
        int c = 1;           // counter variable to keep track of how many values have been read
        vector<double> temp; // temporary vector to store each group of 4 values read from the file
        while (fin >> x)     // loop through the file until there are no more values to read
        {
            temp.push_back(x); // add the current value to the temporary vector
            if ((c % 4) == 0)  // if the temporary vector contains 4 values
            {
                f_vector_data.push_back(temp);        // add the temporary vector to the main vector of vectors
                temp.erase(temp.begin(), temp.end()); // clear the temporary vector to prepare for the next group of 4 values
            }
            c++; // increment the counter variable
        }
        return f_vector_data; // return the main vector of vectors containing all the data from the file
    }
};

// data class
class P_Vector
{
protected:
    double pX, pY, pZ; // protected variables for momentum components

public:
    // Constructor with default values for momentum components
    P_Vector(double x = 0.0, double y = 0.0, double z = 0.0)
    {
        pX = x;
        pY = y;
        pZ = z;
    }

    // Getters for momentum components
    double get_pX()
    {
        return pX;
    }
    double get_pY()
    {
        return pY;
    }
    double get_pZ()
    {
        return pZ;
    }

    // Calculate transverse momentum
    double pT()
    {
        return sqrt(pow(pX, 2) + pow(pY, 2));
    }

    // Calculate azimuthal angle
    double azimuth_angle()
    {
        return atan2(pY, pX);
    }

    // Calculate polar angle
    double polar_angle()
    {
        double p = sqrt(pow(pX, 2) + pow(pY, 2) + pow(pZ, 2)); // net momentum
        return acos(pZ / p);
    }
};

class FourVector : public P_Vector
{
protected:
    double e; // energy component

public:
    // Constructor with default values for momentum and energy components
    FourVector(double pX = 0.0, double pY = 0.0, double pZ = 0.0, double e1 = 0.0) : P_Vector(pX, pY, pZ)
    {
        e = e1;
    }

    // Getter for energy component
    double get_e()
    {
        return e;
    }

    // Method to update momentum and energy components
    void update(double x, double y, double z, double e1)
    {
        pX = x;
        pY = y;
        pZ = z;
        e = e1;
    }

    // Method to calculate mass of the particle represented by the four-vector
    double calcMass()
    {
        double p = pow(pX, 2) + pow(pY, 2) + pow(pZ, 2); // Calculate the squared magnitude of the momentum vector
        double mass = sqrt(pow(e, 2) - p);               // Calculate the mass using the energy and momentum components in MeV/c^2
        return mass;
    }
};

// This function initializes a map with keys ranging from minx to maxx with bins number of equally spaced bins.
// The values of all the keys are initialized to 0.
void intilizeMap(map<double, double> &mp, double minx, double maxx, int bins)
{
    double width = (maxx - minx) / bins; // calculating the width of each bin
    for (double i = minx; i < maxx; i += width)
    {
        mp.insert({i, 0}); // inserting key-value pairs into the map
    }
}

// This function updates the value of the corresponding key in the map by incrementing it by 1
// if the value of x falls within the range minx and maxx.
void mapUpdate(double x, map<double, double> &mp, double minx, double maxx)
{
    if (x >= minx && x < maxx)                                             // checking if x falls within the range minx and maxx
        if ((mp.find(x) != mp.end()) && (mp.lower_bound(x) == mp.find(x))) // checking if x is already a key in the map
        {
            mp[x]++; // incrementing the value of the key by 1
        }
        else
        {

            auto it = mp.lower_bound(x); // getting the iterator to the key that is greater than or equal to x
            it--;                        // moving the iterator to the key that is less than x
            (*it).second++;              // incrementing the value of the key by 1
        }
}

// This function takes a reference to a map as input and updates the values of the map to probabilities.
void probabilityMap(map<double, double> &mp)
{
    int sum = 0;       // initializing a variable to store the sum of all values in the map
    for (auto it : mp) // iterating through the map and adding up all the values
    {
        sum += it.second;
    }
    auto it = mp.begin();          // getting an iterator to the beginning of the map
    for (it; it != mp.end(); it++) // iterating through the map
    {
        (*it).second = (double)(*it).second / sum; // updating the value of the key by dividing it with the sum of all values
    }
}

// This function takes a map and the maximum value of the x-axis as input and generates a histogram file.
void generate_HistFile(map<double, double> mp, double maxx)
{
    // creating output file and storing probabilities corresponding to bins
    ofstream fout;                             // creating an output file stream
    fout.open("histogram.txt", ios_base::out); // opening the file for writing
    for (auto it : mp)                         // iterating through the map
    {
        fout << it.first << " " << it.second; // writing the key-value pair to the file
        fout << endl;                         // writing a new line character to the file
    }
    fout << maxx; // writing the maximum value of the x-axis to the file
    cout << "\n   histogram.txt file has been generated\n";
}

int main()
{
#pragma region inputbyUser
    // input by user
    cout << "\n*************************************************\n\tWelcome to 1D Histogram Generator\n*************************************************\n";

    string file_loc;
    cout << "\n - Enter the Input File Location \n   >> ";
    cin >> file_loc;
    // selecting distribution
    cout << "\n - Enter the distribution name (like pT,pX,pY,pZ,energy,mass) :";
    // cout << "\n \t1> pT";
    // cout << "\n \t2> pX";
    // cout << "\n \t3> pY";
    // cout << "\n \t4> pZ";
    // cout << "\n \t5> Energy";
    // cout << "\n \t6> Mass";
    cout << "\n   >>";
    string d_name;
    cin >> d_name;
    transform(d_name.begin(), d_name.end(), d_name.begin(), ::tolower);
    double minx, maxx, noOfBins;
    cout << "\n - Enter the number of bins \n   >> ";
    cin >> noOfBins;
    cout << "\n - Enter minimum value of x \n   >> ";
    cin >> minx;
    cout << "\n - Enter maximum value of x \n   >> ";
    cin >> maxx;
    //
#pragma endregion

    //

#pragma region read data file and map intialization
    // read data file
    reader r;
    vector<vector<double>> vec = r.readDatFile(file_loc);

    FourVector f;

    // map to store histogram
    map<double, double> mp;
    intilizeMap(mp, minx, maxx, noOfBins);
#pragma endregion

    if (d_name == "pt") // Tranverse Momentum distribution
    {
        for (int i = 0; i < vec.size(); i++)
        {
            f.update(vec[i][0], vec[i][1], vec[i][2], vec[i][3]);
            mapUpdate(f.pT(), mp, minx, maxx);
        }
        probabilityMap(mp);
        generate_HistFile(mp, maxx);
    }
    else if (d_name == "px") // pX distribution
    {
        for (int i = 0; i < vec.size(); i++)
        {
            f.update(vec[i][0], vec[i][1], vec[i][2], vec[i][3]);
            mapUpdate(f.get_pX(), mp, minx, maxx);
        }
        probabilityMap(mp);
        generate_HistFile(mp, maxx);
    }

    else if (d_name == "py") // pY distribution
    {
        for (int i = 0; i < vec.size(); i++)
        {
            f.update(vec[i][0], vec[i][1], vec[i][2], vec[i][3]);
            mapUpdate(f.get_pY(), mp, minx, maxx);
        }
        probabilityMap(mp);
        generate_HistFile(mp, maxx);
    }

    else if (d_name == "pz") // pZ distribution
    {
        for (int i = 0; i < vec.size(); i++)
        {
            f.update(vec[i][0], vec[i][1], vec[i][2], vec[i][3]);
            mapUpdate(f.get_pZ(), mp, minx, maxx);
        }
        probabilityMap(mp);
        generate_HistFile(mp, maxx);
    }

    else if (d_name == "energy") // energy distribution
    {
        for (int i = 0; i < vec.size(); i++)
        {
            f.update(vec[i][0], vec[i][1], vec[i][2], vec[i][3]);
            mapUpdate(f.get_e(), mp, minx, maxx);
        }
        probabilityMap(mp);
        generate_HistFile(mp, maxx);
    }

    else if (d_name == "mass") // mass distribution
    {
        for (int i = 0; i < vec.size(); i++)
        {
            f.update(vec[i][0], vec[i][1], vec[i][2], vec[i][3]);
            double m = f.calcMass();
            mapUpdate(m, mp, minx, maxx);
        }
        probabilityMap(mp);
        generate_HistFile(mp, maxx);
    }

    else
    {
        cout << "\n Specified distribution not found !!";
        getch();
    }

    getch();
}
#pragma endregion
