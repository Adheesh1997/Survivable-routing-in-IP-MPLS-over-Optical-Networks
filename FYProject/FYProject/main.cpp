#include <iostream>
#include <string>
#include <vector>

#include "files.h"
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
        for (size_t j = 0; j < sp.size(); j++)
        {
            cout << sp[j] << " --> ";
        }
        cout << endl;
    }
}
/************************************/


int main()
{
    files myfile;
    int numOfNodes; //Variable to store number of nodes in the network
    vector<vector<int>> adjacencyMetrix; //Vector to store adjacency metrix that represent netork

    //graph input file location
    string fileLocation = "graph_inputs/05/graph05.csv"; 
    

    //Read csv file and assign values to the matrix 
    if(myfile.readGraphInputFile(numOfNodes, adjacencyMetrix,fileLocation))
    { 
        //myfile.writeLog((fileLocation + " Graph is imported."));
        //If there is no any error while reading file then graph is created
        fiberLinkNetwork graph1(numOfNodes, 40);
        graph1.setupFiberLinkNetwork(adjacencyMetrix);

        //myfile.writeLog("Physical network is created.");
        graph1.printGraph();

        cout << endl;


        /**************** To find shortest path(testing) *********************/
        vector<waveLengthNetworks> waveLengthNetwork = setupWaveLengthNetworks(adjacencyMetrix, 40);

        int vexnum = 14;
        int source = 0;
        int destination = 5;

        findPathDetails pathDetails =  initialize(vexnum, waveLengthNetwork, source, destination);


        cout << "Primary path can create  --->  " << pathDetails.canCreatPP << endl; //true or false
        cout << "BackUp path can create   --->  " << pathDetails.canCreatBP << endl;

        cout << "Primary path wavelength No --->  " << pathDetails.wavelengthNoPP << endl;
        cout << "BackUp path wavelength No  --->  " << pathDetails.wavelengthNoBP << endl;

        cout << "Primary path" << endl;
        printShortestPath(pathDetails.primaryShortPath); //print shortest path

        cout << "BackUp path" << endl;
        printShortestPath(pathDetails.backUpShortPath); //print shortest path

        lightpathNetwork testNetwork;
        testNetwork.setANewLighpath(pathDetails.primaryShortPath, "20");
        testNetwork.viewAllLighpaths();

        /*********************************************************************/
    }
    
	cin.get();
    return 0;
}

