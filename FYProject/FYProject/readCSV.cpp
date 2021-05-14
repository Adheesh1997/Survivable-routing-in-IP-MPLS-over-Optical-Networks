#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "readCSV.h"

using namespace std;

bool readGraphInputFile(int& numOfNodes, vector<vector<int>>& adjacencyMetrix,string fileLocation)
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
        cout<<"Graph input file failed to open!!!"<<endl;
        return false;
    }
}