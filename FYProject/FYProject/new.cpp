#include <iostream>
#include <string>
#include <vector>
#include "readCSV.h"
#include "graph.h"

using namespace std;


int main()
{
    int numOfNodes; //Variable to store number of nodes in the network
    vector<vector<int>> adjacencyMetrix; //Vector to store adjacency metrix that represent netork

    //graph input file location
    string fileLocation = "Graph_inputs/05/graph05.csv"; 
    
    //Read csv file and assign values to the matrix 
    if(readCSVFile(numOfNodes, adjacencyMetrix,fileLocation))
    {
        //If there is no any error while reading file then graph is created
        Graph graph1(numOfNodes);
        graph1.constructGraph(adjacencyMetrix);
        graph1.printGraph();

    }
    

    return 0;
}

