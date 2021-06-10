#include <iostream>
#include <string>
#include <vector>
#include "readCSV.h"
#include "graph.h"
#include "lightpathSettingup.h"
#include "Dijkstra.h"

using namespace std;

/**************** Testing **********************/
void printShortestPath(vector<int> sp) {
    if (sp[0] == -1) {
        cout << "There is no shortest path" << endl;
    }
    else {
        for (int j = 0; j < sp.size(); j++)
        {
            cout << sp[j] << " --> ";
        }
        cout << endl;
    }
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


        /**************** To find shortest path(testing) *********************/
        vector<waveLengthNetworks> waveLengthNetwork = setupWaveLengthNetworks(adjacencyMetrix, 40);

        int vexnum = 14;
        int source = 0;
        int destination = 5;

        vector<int> shortest_path =  initialize(vexnum, waveLengthNetwork, source, destination);
        int waveLengthnumber = shortest_path.back() - 100;
        shortest_path.pop_back();

        cout << waveLengthnumber << endl;

        printShortestPath(shortest_path); //print shortest path

        /*********************************************************************/
    }
    
	cin.get();
    return 0;
}

