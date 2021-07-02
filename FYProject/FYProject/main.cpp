#include <iostream>
#include <string>
#include <vector>

#include "files.h"
#include "graph.h"
#include "lightpathSettingup.h"
#include "Dijkstra.h"
#include "lspRequestGenarator.h"

using namespace std;

/**************** Testing **********************/
/* void printShortestPath(vector<int> sp) {
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
} */
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
        myfile.writeLog((fileLocation + " Graph is imported."));
        //If there is no any error while reading file then graph is created
        fiberLinkNetwork physicalLinkNetwork(numOfNodes, 40);
        physicalLinkNetwork.setupFiberLinkNetwork(adjacencyMetrix);

        myfile.writeLog("Physical network is created.");
        physicalLinkNetwork.printGraph();

        lightpathNetwork waveLengthNetwork;

        lspRequestGenarator lspReqGen(numOfNodes);
        lspRequest lspReq;
        
        lspReq = lspReqGen.generateRequest();
        myfile.writeLog(("New request. Bandwidth = "+to_string(lspReq.bandwidthSize)+",source = "+to_string(lspReq.srcNode)+", Dst = "
                        +to_string(lspReq.DstNode)+", id = "+to_string(lspReq.id)+", request = "+to_string(lspReq.request)));


        if(waveLengthNetwork.checkForAvilableLightpath(lspReq.srcNode,lspReq.DstNode))
        {
            
        }
        /**************** To find shortest path(testing) *********************/
        /* vector<waveLengthNetworks> waveLengthNetwork = setupWaveLengthNetworks(adjacencyMetrix, 40);

        int vexnum = 14;
        int source = 0;
        int destination = 5;

        vector<int> shortest_path =  initialize(vexnum, waveLengthNetwork, source, destination);
        int waveLengthnumber = shortest_path.back() - 100;
        shortest_path.pop_back();

        cout << waveLengthnumber << endl;

        printShortestPath(shortest_path); //print shortest path

        lightpathNetwork testNetwork;
        testNetwork.setANewLighpath(shortest_path, "20");
        testNetwork.viewAllLighpaths(); */
        /*********************************************************************/
    }
    
	cin.get();
    return 0;
}

