#ifndef FILES_H
#define FILES_H

#include<vector>
#include<string>
#include <fstream>

#include "eventDrivenSimulator.h"

using namespace std;



class files
{
private:
    ofstream lspRequests;
    ofstream logFile;

    string currentTime();
    string currentDate();
    string outputFileName();


public:
    files();

    bool readGraphInputFile(int& numOfNodes, vector<vector<int>>& adjacencyMetrix,string fileLocation);
    void writeLog(string message);
    void wrteALSP(events event);
    void readLSPs(string fileLocation, vector<events>& listOfEvents);

    ~files();
};

#endif