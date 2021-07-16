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

void printVec(vector<vector<int>> arr)
{
    for(vector<int> i:arr)
    {
        for(int j:i)
        {
            cout<<j<<" ";
        }cout<<endl;
    }
}
/************************************/

bool establishLSP();

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

        vector<waveLengthNetworks> subWaveNetworks = setupWaveLengthNetworks(adjacencyMetrix, 40);

        waveLengthNetworks defaulSubWaveNetworks = subWaveNetworks[0];
        
        for(vector<int> i:defaulSubWaveNetworks.waveAdjacancyMatrix)
        {
            for(int j: i)
            {
                cout<<j<<" ";
            }
            cout<<endl;
        }
        
        lightpathNetwork waveLengthNetwork;

        lspRequestGenarator lspReqGen(numOfNodes);
        lspRequest lspReq;

        /*******************************************************************************************************************/
        // From Leni default defaulSubWaveNetworks;
        int source = 0;
        int destination = 5;
        int vexnum = 14;
        vector<int> defaultShortPath = defaultPath(14, defaulSubWaveNetworks, source, destination);
        cout << "00000000000000000000000000000000000000000000000000000000000" << endl;
        printShortestPath(defaultShortPath); //print shortest path 
        cout << "00000000000000000000000000000000000000000000000000000000000" << endl;

        //From Leni(eka lightpath ekak thiyena kota remaiing kotasata shaortest path eka)

        forRemainingPath createRemaingDeatils = createRemaing( vexnum, subWaveNetworks,  source,  destination);
        cout << "555555555555555555555555555555555555555555555555555555555555555555555555" << endl;
        cout << createRemaingDeatils.canCreatRemainPath << endl; // ithiri hariyata hadanna puluwanda baida kiyala
        cout << createRemaingDeatils.wavelengthRemainigNo << endl; // wavelenth Number
        printShortestPath(createRemaingDeatils.wavelengthRemainigPath); // print path
        cout << "555555555555555555555555555555555555555555555555555555555555555555555555" << endl;

        //From leni combine two wavelength
        combineWavelength combineWavelengthDetails = pathCombinationCreat(vexnum, subWaveNetworks,  source,  destination);
        cout << "444444444444444444444444444444444444444444444444444444444444444444444444" << endl;
        cout << combineWavelengthDetails.canCreatCombination << endl; //wavelength dekak connect karanna puluwanda baida kiyala balanawa
        cout << combineWavelengthDetails.wavelengthNo1 << endl; //from source wavlength number
        cout << combineWavelengthDetails.wavelengthNo2 << endl; //to destination wavlength number
        printShortestPath(combineWavelengthDetails.w1ShortPath); // print path from source
        printShortestPath(combineWavelengthDetails.w2ShortPath); // print from intermediate node
        cout << "444444444444444444444444444444444444444444444444444444444444444444444444" << endl;
        /*******************************************************************************************************************/
        
        for(int i = 0; i < 10; )
        {
            lspReq = lspReqGen.generateRequest();
            
            if(lspReq.request)
            {
                i++;
                myfile.writeLog(("New request. Bandwidth = "+to_string(lspReq.bandwidthSize)+",source = "+to_string(lspReq.srcNode)+", Dst = "
                                +to_string(lspReq.DstNode)+", id = "+to_string(lspReq.id)+", request = "+to_string(lspReq.request)));

                cout << endl;
                

                vector<vector<int>> adjMetrixForPrimaryLSP = waveLengthNetwork.lpPAdjacencyMetrix(lspReq.bandwidthSize, numOfNodes);
                
                int vexnum = numOfNodes;
                int source = lspReq.srcNode;
                int destination = lspReq.DstNode;

                findPathDetails pathDetails = startingPoint(vexnum, subWaveNetworks, source, destination,adjMetrixForPrimaryLSP);

                //cout << "already P_LSP is established  ---> "<<pathDetails.alreadyPPhave << endl;

                if (pathDetails.alreadyPPhave == false) 
                {
                    if(!pathDetails.canCreatPP)
                    {
                        myfile.writeLog("Primary LSP path can not establish for this request.");
                        //break;
                    }

                    if(!pathDetails.canCreatBP)
                    {
                        myfile.writeLog("Backup LSP path can not establish for this request.");
                        //break;
                    }

                    if(pathDetails.canCreatBP && pathDetails.canCreatPP)
                    {
                        waveLengthNetwork.setANewLighpath(pathDetails.primaryShortPath,to_string(pathDetails.wavelengthNoPP) );
                        waveLengthNetwork.setANewLighpath(pathDetails.backUpShortPath,to_string(pathDetails.wavelengthNoBP));

                        adjMetrixForPrimaryLSP = waveLengthNetwork.lpPAdjacencyMetrix(lspReq.bandwidthSize, numOfNodes);

                        pathDetails = startingPoint(vexnum, subWaveNetworks, source, destination,adjMetrixForPrimaryLSP);
                        myfile.writeLog("new LP is established");
                    }

                    /* 
                    cout<<"\nfrom if\n";
                    cout << "Primary path can create  --->  " << pathDetails.canCreatPP << endl; //true or false
                    cout << "BackUp path can create   --->  " << pathDetails.canCreatBP << endl;

                    cout << "Primary path wavelength No --->  " << pathDetails.wavelengthNoPP << endl;
                    cout << "BackUp path wavelength No  --->  " << pathDetails.wavelengthNoBP << endl;

                    cout << "Primary path" << endl;
                    printShortestPath(pathDetails.primaryShortPath); //print shortest path

                    cout << "BackUp path" << endl;
                    printShortestPath(pathDetails.backUpShortPath); //print shortest path 
                    */
                }

                if(pathDetails.alreadyPPhave == true) 
                {
                    /* cout<<"\nFrom else\n";
                    cout << "Primary path can create  --->  " << pathDetails.tempCanCreatPP << endl; //true or false

                    cout << "Primary path wavelength No --->  " << pathDetails.tempWavelengthNoPP << endl;

                    cout << "Primary path" << endl;
                    printShortestPath(pathDetails.tempPrimaryShortPath); //print temparary shortest path

                    cout << "vector Size ----> " << pathDetails.tempPrimaryShortPath.size() << endl; */
                    myfile.writeLog("New lsp is created.");
                    /// LSP creat function should call here
                
                }
                
            }
        }
        waveLengthNetwork.viewAllLighpaths();
        

    }
    
	cin.get();
    return 0;
}

