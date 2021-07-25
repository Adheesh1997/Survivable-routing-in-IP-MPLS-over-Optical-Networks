#include <iostream>
#include <string>
#include <vector>

#include "files.h"
#include "graph.h"
#include "lightpathSettingup.h"
#include "Dijkstra.h"
#include "lspRequestGenarator.h"
#include "eventDrivenSimulator.h"

using namespace std;

void printMap(map<int,vector<vector<int>>> arr)
{
    for(auto it = arr.cbegin(); it != arr.cend(); ++it)
    {
        cout<<endl<<it->first<<"-> ";
        
        for(vector<int> j:it->second)
        {
            for(int k:j)
            {
                cout<<k<<" ";
            }cout<<endl;
        }cout<<endl;
    }
}
int main()
{
    bool protectionType = true;//True for bandwisth protection
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
        //physicalLinkNetwork.printGraph();

        vector<waveLengthNetworks> subWaveNetworks = setupWaveLengthNetworks(adjacencyMetrix, 40);

        waveLengthNetworks defaulSubWaveNetworks = subWaveNetworks[0];
        
        lightpathNetwork waveLengthNetwork;

        lspRequestGenarator lspReqGen(numOfNodes);
        lspRequest lspReq;

        int newLPnoldLP = 0;
        int new2LPs = 0;
        int newLP = 0;
        int oldLP = 0;
        int rejected = 0;
        int numOfRequest = 100;
        
        for(int i = 0; i < numOfRequest; )
        {

            //Generte a lsp reqest with src,dst,bandwidth, request or remove
            lspReq = lspReqGen.generateRequest();
            bool isLSPestablish = false;
            
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

                //If there is no LP(S) to establish LSP
                if (pathDetails.alreadyPPhave == false) 
                {
                    //If LP can not creat for primary path
                    if(!pathDetails.canCreatPP)
                    {
                        //Find shortest path between lsp src(2) and dst(5)(ex:- 2->4->7->3->5)
                        vector<int> defaultShortPath = defaultPath(numOfNodes, defaulSubWaveNetworks, source, destination);

                        //Try to creat path using 2 LPs
                        for(int j = defaultShortPath.size()-2; j > 0 ; j--)
                        {
                            //Find LP within shortest path (ex:- 2->4->7)
                            if(waveLengthNetwork.checkForAvilableLightpath(defaultShortPath[0],defaultShortPath[j]))
                            {
                                forRemainingPath createRemaingDeatils = createRemaing( vexnum, subWaveNetworks,  defaultShortPath[j],  defaultShortPath[defaultShortPath.size()-1]);

                                //Check whether for remain part LP can establish or not (ex:- 7->3->5)
                                if(createRemaingDeatils.canCreatRemainPP && createRemaingDeatils.canCreatRemainBP)
                                {
                                    waveLengthNetwork.setANewLighpath(createRemaingDeatils.wavelengthRemainigPP,createRemaingDeatils.wavelengthRemainigPPNo,"pp");
                                    waveLengthNetwork.setANewLighpath(createRemaingDeatils.wavelengthRemainigBP,createRemaingDeatils.wavelengthRemainigBPNo,"pp");

                                    vector<int> temp = waveLengthNetwork.getWaveNumbers(defaultShortPath[0],defaultShortPath[j], numOfNodes, lspReq.bandwidthSize);
                                    
                                    if(temp[0] != -1)
                                    {
                                        vector<int> primaryWaves = {temp[0],createRemaingDeatils.wavelengthRemainigPPNo};
                                        vector<int> backupWaves = {temp[1],createRemaingDeatils.wavelengthRemainigBPNo};

                                        vector<int> pathForLSP = {defaultShortPath[0],defaultShortPath[j],defaultShortPath[defaultShortPath.size()-1]};

                                        waveLengthNetwork.setANewLSP(pathForLSP, primaryWaves, waveLengthNetwork, "pp", lspReq.id, protectionType);
                                        waveLengthNetwork.setANewLSP(pathForLSP, backupWaves, waveLengthNetwork, "bp", lspReq.id, protectionType);

                                        isLSPestablish = true;
                                        myfile.writeLog("LSP established using 1 new LP & 1 old LP.*");
                                        newLPnoldLP++;
                                        break;
                                    }
                                }
                            }
                        }
                    }

                    if(!pathDetails.canCreatPP && !isLSPestablish)
                    {
                        combineWavelength combineWavelengthDetails = pathCombinationCreat(vexnum, subWaveNetworks,  source,  destination);
                        
                        if(combineWavelengthDetails.canCreatCombinationPP && combineWavelengthDetails.canCreatCombinationBP)
                        {
                            //Setup LPs for primary LSP
                            waveLengthNetwork.setANewLighpath(combineWavelengthDetails.w1ShortPathPP,combineWavelengthDetails.wavelengthNo1PP,"pp");
                            waveLengthNetwork.setANewLighpath(combineWavelengthDetails.w2ShortPathPP,combineWavelengthDetails.wavelengthNo2PP,"pp");

                            //Setup primary LSP
                            vector<int> pathForPLSP = {combineWavelengthDetails.w1ShortPathPP[0],combineWavelengthDetails.connectingNodePP,combineWavelengthDetails.w2ShortPathPP[combineWavelengthDetails.w2ShortPathPP.size()-1]};
                            vector<int> wavesP = {combineWavelengthDetails.wavelengthNo1PP,combineWavelengthDetails.wavelengthNo2PP};
                            waveLengthNetwork.setANewLSP(pathForPLSP, wavesP, waveLengthNetwork, "pp", lspReq.id, protectionType);

                            waveLengthNetwork.setANewLighpath(combineWavelengthDetails.w1ShortPathBP, combineWavelengthDetails.wavelengthNo1BP,"pp");
                            waveLengthNetwork.setANewLighpath(combineWavelengthDetails.w2ShortPathBP, combineWavelengthDetails.wavelengthNo2BP, "pp");

                            vector<int> pathForBLSP = {combineWavelengthDetails.w1ShortPathBP[0],combineWavelengthDetails.connectingNodeBP,combineWavelengthDetails.w2ShortPathBP[combineWavelengthDetails.w2ShortPathBP.size()-1]};
                            vector<int> wavesB = {combineWavelengthDetails.wavelengthNo1BP, combineWavelengthDetails.wavelengthNo2BP};

                            waveLengthNetwork.setANewLSP(pathForBLSP, wavesB, waveLengthNetwork, "bp", lspReq.id, protectionType);

                            myfile.writeLog("New lsp establish with 2 new LPs.**");
                            new2LPs++;
                            isLSPestablish = true;

                        }
                    }

                    //If using new LPs can establish path for LSP reqeust
                    if(pathDetails.canCreatBP && pathDetails.canCreatPP && !isLSPestablish)
                    {
                        waveLengthNetwork.setANewLighpath(pathDetails.primaryShortPath, pathDetails.wavelengthNoPP,"pp" );
                        waveLengthNetwork.setANewLighpath(pathDetails.backUpShortPath,pathDetails.wavelengthNoBP,"pp");

                        vector<int> pathForPLSP = {pathDetails.primaryShortPath[0],pathDetails.primaryShortPath[pathDetails.primaryShortPath.size()-1]};
                        vector<int> wavesP = {pathDetails.wavelengthNoPP};
                        waveLengthNetwork.setANewLSP(pathForPLSP, wavesP, waveLengthNetwork, "pp", lspReq.id, protectionType);

                        vector<int> pathForBLSP = {pathDetails.backUpShortPath[0],pathDetails.backUpShortPath[pathDetails.backUpShortPath.size()-1]};
                        vector<int> wavesB = {pathDetails.wavelengthNoBP};
                        waveLengthNetwork.setANewLSP(pathForBLSP, wavesB, waveLengthNetwork, "bp", lspReq.id, protectionType);

                        isLSPestablish = true;
                        myfile.writeLog("New lsp established with new single LP.***");
                        newLP++;
                    }
                }

                //Already there is path(LP) for LSP request
                else
                {  
                    if(pathDetails.tempCanCreatPP)
                    {
                        vector<int> waveVector = waveLengthNetwork.getWaveNumbers(source,destination,numOfNodes, lspReq.bandwidthSize, pathDetails.tempPrimaryShortPath.size());
                        
                        if(waveVector[0] == -1)
                        {
                            //find a backup path
                            int backupWave = waveLengthNetwork.getBWaveNumber(source,destination,numOfNodes,lspReq.bandwidthSize,pathDetails.tempPrimaryShortPath);
                            if(backupWave != -1)
                            {
                                waveLengthNetwork.setANewLighpath(pathDetails.tempPrimaryShortPath,pathDetails.tempWavelengthNoPP,"pp");
                                subWaveNetworks[pathDetails.tempWavelengthNoPP].removeLink(source,destination);

                                vector<int> pathForLSP = {source,destination};
                                vector<int> wavesP = {pathDetails.tempWavelengthNoPP};
                                vector<int> wavesB = {backupWave};

                                waveLengthNetwork.setANewLSP(pathForLSP, wavesP, waveLengthNetwork, "pp", lspReq.id, protectionType);
                                waveLengthNetwork.setANewLSP(pathForLSP, wavesB, waveLengthNetwork, "bp", lspReq.id, protectionType);

                                myfile.writeLog("New lsp establish with new LP and old LP.");
                                newLPnoldLP++;
                                isLSPestablish = true;
                            }
                        }
                        else
                        {
                            vector<int> pathForLSP = {source,destination};
                            vector<int> wavesP = {waveVector[0]};
                            vector<int> wavesB = {waveVector[1]};

                            waveLengthNetwork.setANewLSP(pathForLSP,wavesP,waveLengthNetwork,"pp",lspReq.id, protectionType);
                            waveLengthNetwork.setANewLSP(pathForLSP,wavesB,waveLengthNetwork,"bp",lspReq.id, protectionType);
                            
                            myfile.writeLog("New lsp established with 2 old LPs.");
                            oldLP++;
                            isLSPestablish = true;
                        }
                    }                
                }

                if(i == 99)
                {
                    cout<<endl<<endl;
                    cout<<"source = "<<lspReq.srcNode<<" , "<<"dest = "<<lspReq.DstNode<<endl;
                    cout<<"\nmap from src";
                    map<int,vector<vector<int>>> arr = waveLengthNetwork.mapFromsource(lspReq.srcNode,numOfNodes);
                    printMap(arr);
                    cout<<"\nmap from dst\n";
                    map<int,vector<vector<int>>> arr1 = waveLengthNetwork.mapFromdst(lspReq.DstNode,numOfNodes);
                    printMap(arr1);
                }

                if(!isLSPestablish) 
                {
                    rejected++;
                    myfile.writeLog("LSP is REJECTED.");
                }
                
                
            }

        }

        myfile.writeLog("                 ");
        myfile.writeLog("****Counts****");
        myfile.writeLog("Num of lsp rqst = "+to_string(numOfRequest));
        myfile.writeLog("new LP & old LP = "+to_string(newLPnoldLP));
        myfile.writeLog("new 2 LPs  = "+to_string(new2LPs));
        myfile.writeLog("new LP = "+to_string(newLP));
        myfile.writeLog("old LP = "+to_string(oldLP));
        myfile.writeLog("                 ");
        myfile.writeLog("**Num.of LSP established = "+to_string(newLPnoldLP+new2LPs+newLP+oldLP));
        myfile.writeLog("**Num.of LSP rejected = "+to_string(rejected));
    }

    int numberOfLSPrequests = 100000;

    requestCreation tempObject;
    //tempObject.requestGenerator(numberOfLSPrequests);
    //tempObject.eventCreation();
    
	cin.get();
    return 0;
}
