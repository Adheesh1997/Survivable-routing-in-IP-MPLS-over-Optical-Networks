#include <iostream>
#include <string>
#include <vector>
#include "readCSV.h"
#include "graph.h"
#include "LPrelease.h"

using namespace std;

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
        fiberLinkNetwork graph1(numOfNodes);
        graph1.setupFiberLinkNetwork(adjacencyMetrix);
        graph1.printGraph();

        // Adjacency matrices for different wave lengths save here
        vector<waveLengthNetworks> waveLengthNetwork = setupWaveLengthNetworks(adjacencyMetrix,40);

    }
    
	cin.get();
    return 0;
}

