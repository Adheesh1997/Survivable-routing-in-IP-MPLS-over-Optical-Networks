#include <iostream>
#include <string>
#include <vector>
#include "readCSV.h"
#include "graph.h"
#include "lightpathSettingup.h"

using namespace std;

/***************** testing*****************/
void func(vector<vector<int>> adjacencyMetrix)
{

    vector<waveLengthNetworks> waveLengthNetwork = setupWaveLengthNetworks(adjacencyMetrix,40);
    waveLengthNetwork[0].waveAdjacancyMatrix;


    cout<<waveLengthNetwork[0].waveAdjacancyMatrix[0][1]<<"befor\n";
    waveLengthNetwork[0].removeLink(0,1);
    cout<<waveLengthNetwork[0].waveAdjacancyMatrix[0][1]<<"after\n";

}
/************************************/

int main()
{
    int numOfNodes; //Variable to store number of nodes in the network
    vector<vector<int>> adjacencyMetrix; //Vector to store adjacency metrix that represent netork

    //graph input file location
    string fileLocation = "graph_inputs/05/graph05.csv"; 
    

    //Read csv file and assign values to the matrix 
    if(readGraphInputFile(numOfNodes, adjacencyMetrix,fileLocation))
    { 
        //If there is no any error while reading file then graph is created
        fiberLinkNetwork graph1(numOfNodes, 40);
        graph1.setupFiberLinkNetwork(adjacencyMetrix);
        graph1.printGraph();


        //testing
        func(adjacencyMetrix);   
    
    }
    
	cin.get();
    return 0;
}

