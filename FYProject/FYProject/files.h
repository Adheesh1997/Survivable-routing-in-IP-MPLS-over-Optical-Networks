#ifndef FILES_H
#define FILES_H

#include<vector>
#include<string>
#include <fstream>

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

    bool readGraphInputFile(int& numOfNodes, vector<vector<int>>& adjacencyMetrix,string fileLocation);
    void writeLog(string message);

    ~files();
};

#endif