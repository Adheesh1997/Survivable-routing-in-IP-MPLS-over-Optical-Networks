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

        cout << endl;

        vector<int> shortestPathForLSP;
        /* if(shortestPathForLSP =lspShortestPath(lspReq.srcNode,lspReq.DstNode,lspReq.bandwidthSize))
        {
            waveLengthNetwork.setANewLSP(shortestPathForLSP,to_string(lspReq.bandwidthSize),waveLengthNetwork);
        } */
        //else
        {
            
        }

        /**************** To find shortest path(testing) *********************/
        vector<waveLengthNetworks> subWaveNetworks = setupWaveLengthNetworks(adjacencyMetrix, 40);

        int vexnum = 14;
        int source = lspReq.srcNode;
        int destination = lspReq.DstNode;

        findPathDetails pathDetails =  initialize(vexnum, subWaveNetworks, source, destination);


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
        testNetwork.setANewLighpath(pathDetails.backUpShortPath,"20");
        testNetwork.viewAllLighpaths();

        /*********************************************************************/

        /************************ for leni ***********************************/
        vector<vector<int>> adjMetrixForPrimaryLSP = testNetwork.lpPAdjacencyMetrix(lspReq.bandwidthSize, numOfNodes);
        /// if shortest path for primary LSP is temp
        vector<int> primaryPath = {1,3,5};

        vector<vector<int>> adjMetrixForBackupLSP = testNetwork.lpBAdjacencyMetrix(primaryPath,numOfNodes);

        /***************************** end of for leni ******************************/

    }
    
	cin.get();
    return 0;
}

