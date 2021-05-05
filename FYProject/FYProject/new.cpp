#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

void readCSVFile(int& numOfNodes, vector<vector<int>>& adjacencyMetrix,string fileLocation)
{
    ifstream myFileStream(fileLocation);

    if(myFileStream.is_open())
    {

        string line1;
        
        getline(myFileStream,line1);//Read first line of the csv file

        numOfNodes = stoi(line1);//Assign the value to the variable
        cout<<"Num of nodes = "<<numOfNodes<<endl<<endl;


        string rowOfMatrix;

        while(getline(myFileStream,rowOfMatrix))
        {
            vector<int> temp;
            stringstream line(rowOfMatrix);
            string num;

            while(getline(line,num,','))
            {
                temp.push_back(stoi(num));
            }

            adjacencyMetrix.push_back(temp);
        }
        
        myFileStream.close();
    }
    else
    {
        cout<<"Graph input file failed to open!!!"<<endl;
        return;
    }
}

int main()
{
    int numOfNodes;
    vector<vector<int>> adjacencyMetrix;

    string fileLocation = "Graph_inputs/03/graph03.csv";
    
    readCSVFile(numOfNodes, adjacencyMetrix,fileLocation);//Read csv file and assign values to the metrix

    for(int i=0;i<numOfNodes;i++) //Print the adjacency metrix
    {
        for(int j=0;j<numOfNodes;j++)
        {
            cout<<adjacencyMetrix[i][j]<<" ";
        }
        cout<<endl;
    }

    return 0;
}