#define _CRT_SECURE_NO_WARNINGS
#include "files.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <time.h>

#include "eventDrivenSimulator.h"

using namespace std;

/*files::files()
{
    logFile.open(("log_files/"+outputFileName()),ios_base::app);
    if(logFile.is_open())
        logFile<<currentTime()<<" "<<"Log file is created\n";
}*/

void files::createLog()
{
    logFile.open(("log_files/" + outputFileName()), ios_base::app);
    if (logFile.is_open())
        logFile << currentTime() << " " << "Log file is created\n";
}

bool files::readGraphInputFile(int& numOfNodes, vector<vector<int>>& adjacencyMetrix,string fileLocation, int numOfWaves)
{
    /*
    Parameters_
        1. numOfNodes - number of nodes that network has,that taken from csv file
        2. adjacencyMetrix - network input that taken from csv file
        3. fileLocation - location of the csv file that contain network input

    Return value_
        If file was read successfully and no any error then return true,
        If faild to read file return false 
    */

   ifstream myFileStream(fileLocation);//Create ifstream file object using file location

    //Check whether file is open without any error
    if(myFileStream.is_open())
    {
        
        string line1;  //String to take first line of csv(num.of nodes)
        
        getline(myFileStream,line1);  //Read first line of the csv file

        numOfNodes = stoi(line1);  //Convert string into interger and assign the value to the variable

        string rowOfMatrix;  //String to take line of the matrix

        //Read line and copy to rowOfMatrix
        while(getline(myFileStream,rowOfMatrix)) 
        {
            vector<int> temp;
            stringstream line(rowOfMatrix); //rowOfMatrix stream into line 
            string num;

            while(getline(line,num,',')) //Separate values from line and copy to num
            {
                temp.push_back(stoi(num)); //Convert num into integer and store in temprory vector                 
            }

            adjacencyMetrix.push_back(temp); //Push temprary vector that contain a row of adjacency metrix to final vector
        }
        
        myFileStream.close(); //close the file
        return true; 
    }

    else    // If file does not open output error message and return from the function
    {
        cout<<"Err! Graph input file failed to open!!!"<<endl;
        return false;
    }
}

string files::currentTime() 
{
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    //strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    strftime(buf, sizeof(buf), "[%X]", &tstruct);

    return buf;

}

string files::currentDate() 
{
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    //strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);

    return buf;

}

string files::outputFileName() 
{
    string NowTime;
    time_t now;

    struct tm nowLocal;

    now=time(NULL); // get the time from the OS

    nowLocal=*localtime(&now);

    NowTime =   to_string(nowLocal.tm_year+1900)+'-'+to_string(nowLocal.tm_mon+1)+'-'+to_string(nowLocal.tm_mday)+'_'+
                to_string(nowLocal.tm_hour) + '.' + to_string(nowLocal.tm_min)+".txt";
    
    return NowTime;
}

void files::writeLog(string message = " ")
{
    if(logFile.is_open())
    {
        logFile<<currentTime()<<" "<<message<<endl;
    }
}


void files::wrteALSP(string fileLocation, vector<events>& listOfEvents)
{
    ofstream lspRequests;
    lspRequests.open(fileLocation);

    if(lspRequests.is_open())
    {
        for(events event:listOfEvents)
        {
            lspRequests << event.sourceNode << "," << event.destinationNode << "," << event.identifier << "," << event.action << "," << event.bandwidth << endl;
        }
    }
    else cout<<"LSP file not open to write\n";

    lspRequests.close();
}

void files::readLSPs(string fileLocation, vector<events>& listOfEvents)
{
    ifstream myFileStream(fileLocation);

    string rows;

    if(myFileStream.is_open())
    {
        while(getline(myFileStream,rows)) 
        {
            events temp;
            stringstream line(rows); //rowOfMatrix stream into line 
            string num;

            getline(line,num,','); 
            temp.sourceNode = stoi(num);

            getline(line,num,','); 
            temp.destinationNode = stoi(num);

            getline(line,num,','); 
            temp.identifier = stoi(num);

            getline(line,num,','); 
            temp.action = stoi(num);

            getline(line, num, ',');
            temp.bandwidth = stoi(num);

            listOfEvents.push_back(temp); //Push temprary vector that contain a row of adjacency metrix to final vector
        }

        myFileStream.close();
    }
    else cout<<"\nlsp Request read from file failed, check file location!!!\n";
        
}




files::~files()
{
   logFile.close();
}
