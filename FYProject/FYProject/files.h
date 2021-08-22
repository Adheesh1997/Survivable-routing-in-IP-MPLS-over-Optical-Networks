#pragma once

#include<vector>
#include<string>
#include <fstream>

#include "eventDrivenSimulator.h"

using namespace std;



class files
{
private:
    ofstream logFile;

    string currentTime();
    string currentDate();
    string outputFileName();


public:
    files();

    bool readGraphInputFile(int& numOfNodes, vector<vector<int>>& adjacencyMetrix,string fileLocation, int numOfWaves);
    void writeLog(string message);
    void wrteALSP(string fileLocation, vector<events>& listOfEvents);
    void readLSPs(string fileLocation, vector<events>& listOfEvents);

    ~files();
};
