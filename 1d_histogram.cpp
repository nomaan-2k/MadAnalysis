/* A program to generate a 1D histogram from a given four vector data file
   The user enters the data file location, distribution name, number of bins,
   minimun value for x-axis, maximum value for x-axis.

   Using the data from input file the program generates a two column text file,
   histogram.txt where first column contains the lower bound of each bin with 
   its corresponding probabilities in the second column. The last row (first column)
   contains the upper bound of the histogram.
*/  

// header files
#include <fstream>      //for file handling
#include <iostream>
#include <conio.h>      //for getch()
#include <vector>       //for vector
#include <cmath>
#include <map>          //for map
#include <algorithm>    //for transform

using namespace std;


// reader class
// contains readDataFile member function that reads data from a file and returns it in form of a vector of vectors
class reader
{

private:
    vector<vector<double>> f_vector_data; // a vector of vectors to store the data

public:
    // readData member funtion 
    // takes file locataion as argument
    vector<vector<double>> readDatFile(string fileLoc)
    {

        ifstream fin;             // creates an input file stream object
        fin.open(fileLoc);        // open the file with the name given in the parameter string
        if (!fin.is_open())       // check if the file was opened successfully
        {
            // if the file was not opened successfully, print an error message and exit the program
            cout << "\nNOT ABLE TO READ FILE! Press any key to exit";
            getch();
            exit(0); 
        }

        double x;            // temporary variable to store each value read from the file
        int c = 1;           // counter to keep track of how many values have been read
        vector<double> temp; // temporary vector to store each group of 4 values read from the file
        while (fin >> x)     // loop through the file until there are no more values to read
        {
            temp.push_back(x); // add the current value to the temporary vector

            if ((c % 4) == 0)  // if the temporary vector contains 4 values
            {
                f_vector_data.push_back(temp);        // adds the temporary vector to the main vector of vectors
                temp.erase(temp.begin(), temp.end()); // clears the temporary vector to prepare for the next group of 4 values
            }
            c++;
        }
        return f_vector_data; // returns the main vector of vectors containing all the data from the file
    }
};


// P_Vector class
// stores the momentum components along x,y and z axis
// has member functions to extract transverse momentum, and angles phi and theta
class P_Vector
{
protected:
    double pX, pY, pZ; // protected variables for momentum components

public:
    // constructor with default values for momentum components
    P_Vector(double x = 0.0, double y = 0.0, double z = 0.0)
    {
        pX = x;
        pY = y;
        pZ = z;
    }

    // getter functions for momentum components
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

    // member function to calculate transverse momentum
    double pT()
    {
        return sqrt(pow(pX, 2) + pow(pY, 2));
    }

    // member function to calculate azimuthal angle (phi)
    double azimuth_angle()
    {
        return atan2(pY, pX);
    }

    // member function to calculate polar angle (theta)
    double polar_angle()
    {
        double p = sqrt(pow(pX, 2) + pow(pY, 2) + pow(pZ, 2)); // net momentum
        return acos(pZ / p);
    }
};

// data class (two layered)
// FourVector class has energy data member to store energy, along with pX,pY,pZ inherited from P_Vector
// It has member function that returns mass of the particle represented by the fourvector object
class FourVector : public P_Vector
{
protected:
    double e; // variable to store energy

public:
    // constructor with default values for momentum and energy components
    FourVector(double pX = 0.0, double pY = 0.0, double pZ = 0.0, double e1 = 0.0) : P_Vector(pX, pY, pZ)
    {
        e = e1;
    }

    // Method to update momentum components and energy
    void update(double x, double y, double z, double e1)
    {
        pX = x;
        pY = y;
        pZ = z;
        e = e1;
    }

    // getter function for energy
    double get_e()
    {
        return e;
    }


    // Method to calculate mass of the particle represented by the four-vector
    double calcMass()
    {
        double p = pow(pX, 2) + pow(pY, 2) + pow(pZ, 2); // Calculate the squared magnitude of the momentum vector
        double mass = sqrt(pow(e, 2) - p);               // Calculate the mass using the energy and momentum components in MeV/c^2
        return mass;
    }
};

// Function to initialize a map representing a histogram
// where keys are the lower bounds of each bin of the histogram
// The values of all the keys are initialized to 0.
void intilizeMap(map<double, double> &mp, double minx, double maxx, int n_bins)
{
    double width = (maxx - minx) / n_bins; // calculating the width of each bin
    for (double i = minx; i < maxx; i += width)
    {
        mp.insert({i, 0}); // inserting key-value pairs into the map
    }
}

// Function to update the value of the corresponding key in the map according
// to interval in which the input data x lies

void mapUpdate(double x, map<double, double> &mp, double minx, double maxx)
{
    if (x >= minx && x < maxx)                                             // check if x falls within the range minx and maxx
        if ((mp.find(x) != mp.end()) && (mp.lower_bound(x) == mp.find(x))) // check if x is already a key in the map
        {
            mp[x]++; // incrementing the value of the key by 1
        }
        else
        {

            auto it = mp.lower_bound(x);
            it--;                        // getting the iterator to the key just lower than x
            (*it).second++;              // incrementing the value of the key by 1
        }
}


// Function to update the values of the map to probabilities.
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
        (*it).second = (double)(*it).second / sum; // updating the probability of each value
    }
}

// Function to generate a histogram file from a map
void generate_HistFile(map<double, double> mp, double maxx)
{
    ofstream fout;                             // creating an output file stream
    fout.open("histogram.txt", ios_base::out); // opening the file for writing
    for (auto it : mp)                         // iterating through the map
    {
        fout << it.first << "  " << it.second; // writing the key-value pair to the file
        fout << endl;                          // writing a new line character
    }
    fout << maxx; // adding the upper bound of the histogram to column 1
    cout << "\n   histogram.txt file has been generated\n";
}

// main function

int main()
{

    // Getting inputs from user
    // The inputs include the file location, distribution name, number of bins,
    // minimum and maximum values of x
    // These are used to generate a histogram for the given distribution

    // Get input from user
    cout << "\n*************************************************\n\tWelcome to 1D Histogram Generator\n*************************************************\n";
    
    string file_loc;
    cout << "\n - Enter the Input File Location \n >> ";
    cin >> file_loc;
    string d_name;  //distribution name
    cout << "\n - Enter the distribution name (like pT,pX,pY,pZ,energy,mass) :\n >>";
    cin >> d_name;
    double minx, maxx, noOfBins;
    cout << "\n - Enter the number of bins \n >> ";
    cin >> noOfBins;
    cout << "\n - Enter minimum value of x \n >> ";
    cin >> minx;
    cout << "\n - Enter maximum value of x \n >> ";
    cin >> maxx;

    cout << "\n Generating Histogram ... \n ";



    // read data from input file

    reader r;   //reader object
    // storing four vector data from file in form of a vector of fourVectors
    vector<vector<double>> fvData = r.readDatFile(file_loc);

    // fourvector object to store a four vectors from fvData 
    // and calculate other quantities like mass using member functions
    FourVector f;

    // intialize a map to store histogram
    map<double, double> mp;
    intilizeMap(mp, minx, maxx, noOfBins);

    // Convert the distribution name to lowercase using the transform function 
    // to facilitate case insensitive string comparison
    transform(d_name.begin(), d_name.end(), d_name.begin(), ::tolower);

    if (d_name == "pt") // Tranverse Momentum distribution
    {
        // iterate over each fourvector present in fvData
        // and generate histogram according to values of pT for each fourvector 
        for (int i = 0; i < fvData.size(); i++)
        {
            // assignment of FourVector object f using to ith fourvector of fvData 
            f.update(fvData[i][0], fvData[i][1], fvData[i][2], fvData[i][3]);
            // update the histogram (stored in map mp) according to value of pT of object f
            mapUpdate(f.pT(), mp, minx, maxx);
        }

        // convert y values of generated histogram (stored in map mp) to probabilities
        probabilityMap(mp);
        // copy the histogram data from the map to a text file
        generate_HistFile(mp, maxx);
    }

    else if (d_name == "px") // pX distribution
    {
        // same process as in in pT
        for (int i = 0; i < fvData.size(); i++)
        {
            f.update(fvData[i][0], fvData[i][1], fvData[i][2], fvData[i][3]);
            mapUpdate(f.get_pX(), mp, minx, maxx);
        }
        probabilityMap(mp);
        generate_HistFile(mp, maxx);
    }

    else if (d_name == "py") // pY distribution
    {
        for (int i = 0; i < fvData.size(); i++)
        {
            f.update(fvData[i][0], fvData[i][1], fvData[i][2], fvData[i][3]);
            mapUpdate(f.get_pY(), mp, minx, maxx);
        }
        probabilityMap(mp);
        generate_HistFile(mp, maxx);
    }

    else if (d_name == "pz") // pZ distribution
    {
        for (int i = 0; i < fvData.size(); i++)
        {
            f.update(fvData[i][0], fvData[i][1], fvData[i][2], fvData[i][3]);
            mapUpdate(f.get_pZ(), mp, minx, maxx);
        }
        probabilityMap(mp);
        generate_HistFile(mp, maxx);
    }

    else if (d_name == "energy") // energy distribution
    {
        for (int i = 0; i < fvData.size(); i++)
        {
            f.update(fvData[i][0], fvData[i][1], fvData[i][2], fvData[i][3]);
            mapUpdate(f.get_e(), mp, minx, maxx);
        }
        probabilityMap(mp);
        generate_HistFile(mp, maxx);
    }

    else if (d_name == "mass") // mass distribution
    {
        for (int i = 0; i < fvData.size(); i++)
        {
            f.update(fvData[i][0], fvData[i][1], fvData[i][2], fvData[i][3]);
            mapUpdate(f.calcMass(), mp, minx, maxx);
        }
        probabilityMap(mp);
        generate_HistFile(mp, maxx);
    }

    else
    {
        // Error message if entered distribution is not available
        cout << "\n  ERROR!! Distribution not found !!";
        getch();
    }

    getch();
}
