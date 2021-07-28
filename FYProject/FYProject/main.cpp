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
    files myfile;
    int numOfNodes; //Variable to store number of nodes in the network


    bool protectionType = true;         //True for bandwidth based LP protection. False for number of LSPs based LP protection
    int numberOfLSPrequests = 100;    //The number of LSP requests
    double erlang = 10;                 //Erlang value
    double meanHoldingTime = 1;         //Mean holding time


    requestCreation tempObject;
    tempObject.requestGenerator(numberOfLSPrequests, erlang, meanHoldingTime);   //Create the LSP requests
    //tempObject.printLSPrequests();                                             //Print the LSP requests
    vector<events> listOfEvents = tempObject.eventCreation();                    //Create the events
    //tempObject.printEvents();                                                  //Print the events
    vector<int> rejectedEvents;                                                  //To capture the rejected events
    vector<events>::iterator itr;


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
        
        
        while(!listOfEvents.empty())//for(events event:tempObject.eventVector)
        {
            
            int vexnum = numOfNodes;
            int source = listOfEvents[0].sourceNode;
            int destination = listOfEvents[0].destinationNode;
            int id = listOfEvents[0].identifier;
            int bandwidth = listOfEvents[0].bandwidth;
            bool action = listOfEvents[0].action;

            //Generte a lsp reqest with src,dst,bandwidth, request or remove
            bool isLSPestablish = false;
            
            while (!rejectedEvents.empty())
            {
                int count = 0;
                if (listOfEvents[0].identifier == rejectedEvents[count])
                {
                    itr = listOfEvents.begin();
                    listOfEvents.erase(itr);

                    vector<int>::iterator itr2;
                    itr2 = rejectedEvents.begin() + count;
                    rejectedEvents.erase(itr2);
                }
                count++;
            }

            if(action)
            {
                myfile.writeLog(("New request. Bandwidth = "+to_string(bandwidth)+",source = "+to_string(source)+", Dst = "
                                +to_string(destination)+", id = "+to_string(id)+", request = "+to_string(action)));

                

                vector<vector<int>> adjMetrixForPrimaryLSP = waveLengthNetwork.lpPAdjacencyMetrix(bandwidth, numOfNodes);
                

                findPathDetails pathDetails = startingPoint(vexnum, subWaveNetworks, source, destination,adjMetrixForPrimaryLSP);


                //If there is no LP(S) to establish LSP
                if (pathDetails.alreadyPPhave == false) 
                {
                    //If using new LPs can establish path for LSP reqeust
                    if(pathDetails.canCreatBP && pathDetails.canCreatPP && !isLSPestablish)
                    {
                        myfile.writeLog("New lsp established with new single LP.***");
                        
                        waveLengthNetwork.setANewLighpath(pathDetails.primaryShortPath, pathDetails.wavelengthNoPP,"pp" );
                        waveLengthNetwork.setANewLighpath(pathDetails.backUpShortPath,pathDetails.wavelengthNoBP,"pp");

                        vector<int> pathForPLSP = {pathDetails.primaryShortPath[0],pathDetails.primaryShortPath[pathDetails.primaryShortPath.size()-1]};
                        vector<int> wavesP = {pathDetails.wavelengthNoPP};
                        waveLengthNetwork.setANewLSP(pathForPLSP, wavesP, waveLengthNetwork, "pp", id, protectionType);

                        vector<int> pathForBLSP = {pathDetails.backUpShortPath[0],pathDetails.backUpShortPath[pathDetails.backUpShortPath.size()-1]};
                        vector<int> wavesB = {pathDetails.wavelengthNoBP};
                        waveLengthNetwork.setANewLSP(pathForBLSP, wavesB, waveLengthNetwork, "bp", id, protectionType);

                        isLSPestablish = true;
                        newLP++;
                    }
                    map<int,vector<vector<int>>> arr = waveLengthNetwork.mapFromsource(source,numOfNodes, myfile);
                    map<int,vector<vector<int>>> arr1 = waveLengthNetwork.mapFromsource(destination,numOfNodes, myfile);
                    

                    //If LP can not creat for primary path
                    if(pathDetails.canCreatPP && !pathDetails.canCreatBP && !isLSPestablish)
                    {
                        forRemainingBackUpPath createRemaingBackUpDeatils = createRemaing(vexnum, subWaveNetworks, source, destination, arr, arr1, pathDetails.primaryShortPath);
                        
                        if(createRemaingBackUpDeatils.canCreatCombinationBP)
                        {
                            myfile.writeLog("New LSP establish with single LP for primary, and 2 combine LPs for backup.");
                            
                            if(createRemaingBackUpDeatils.wavelengthNo1BP < 40)
                            {
                                waveLengthNetwork.setANewLighpath(createRemaingBackUpDeatils.w1ShortPathBP,createRemaingBackUpDeatils.wavelengthNo1BP,"pp");
                            }
                            else
                            {
                                createRemaingBackUpDeatils.wavelengthNo1BP -= 40;
                            }

                            if(createRemaingBackUpDeatils.wavelengthNo2BP < 40)
                            {
                                waveLengthNetwork.setANewLighpath(createRemaingBackUpDeatils.w2ShortPathBP,createRemaingBackUpDeatils.wavelengthNo2BP,"pp");
                            }
                            else
                            {
                                createRemaingBackUpDeatils.wavelengthNo2BP -=40;
                            }
                            
                            waveLengthNetwork.setANewLighpath(pathDetails.primaryShortPath, pathDetails.wavelengthNoPP,"pp" );

                            vector<int> pathForPLSP = {pathDetails.primaryShortPath[0],pathDetails.primaryShortPath[pathDetails.primaryShortPath.size()-1]};
                            vector<int> wavesP = {pathDetails.wavelengthNoPP};
                            waveLengthNetwork.setANewLSP(pathForPLSP, wavesP, waveLengthNetwork, "pp", id, protectionType);

                            vector<int> pathForBLSP = {source, createRemaingBackUpDeatils.connectingNodeBP, destination};
                            vector<int> wavesB = {createRemaingBackUpDeatils.wavelengthNo1BP, createRemaingBackUpDeatils.wavelengthNo2BP};

                            waveLengthNetwork.setANewLSP(pathForBLSP,wavesB,waveLengthNetwork,"bp",id,protectionType);

                            isLSPestablish = true;
                            newLP++;

                        }
                    }

                    if(!pathDetails.canCreatPP && !pathDetails.canCreatBP && !isLSPestablish)
                    {
                        combineWavelength combineWavelengthDetails = pathCombinationCreat(vexnum, subWaveNetworks, source, destination, arr, arr1);
                        
                        if(combineWavelengthDetails.canCreatCombinationPP && combineWavelengthDetails.canCreatCombinationBP)
                        {
                            myfile.writeLog("New LSP establish with combine LPs for both primary and backup LSPs.");
                            if(combineWavelengthDetails.wavelengthNo1PP < 40)
                                waveLengthNetwork.setANewLighpath(combineWavelengthDetails.w1ShortPathPP, combineWavelengthDetails.wavelengthNo1PP, "pp");
                            
                            else combineWavelengthDetails.wavelengthNo1PP -= 40;

                            if(combineWavelengthDetails.wavelengthNo2PP < 40)
                                waveLengthNetwork.setANewLighpath(combineWavelengthDetails.w2ShortPathPP, combineWavelengthDetails.wavelengthNo2PP, "pp");
                            
                            else combineWavelengthDetails.wavelengthNo2PP -= 40;


                            if(combineWavelengthDetails.wavelengthNo1BP < 40)
                                waveLengthNetwork.setANewLighpath(combineWavelengthDetails.w1ShortPathBP, combineWavelengthDetails.wavelengthNo1BP, "pp");
                            
                            else combineWavelengthDetails.wavelengthNo1BP -= 40;

                            if(combineWavelengthDetails.wavelengthNo2BP < 40)
                                waveLengthNetwork.setANewLighpath(combineWavelengthDetails.w2ShortPathBP, combineWavelengthDetails.wavelengthNo2BP, "pp");
                            
                            else combineWavelengthDetails.wavelengthNo2BP -= 40;

                            vector<int> pathForPLSP = {source, combineWavelengthDetails.connectingNodePP, destination};
                            vector<int> wavesP = {combineWavelengthDetails.wavelengthNo1PP, combineWavelengthDetails.wavelengthNo2PP};

                            waveLengthNetwork.setANewLSP(pathForPLSP, wavesP, waveLengthNetwork, "pp", id, protectionType);

                            vector<int> pathForBLSP = {source, combineWavelengthDetails.connectingNodeBP, destination};
                            vector<int> wavesB = {combineWavelengthDetails.wavelengthNo1BP, combineWavelengthDetails.wavelengthNo2BP};

                            waveLengthNetwork.setANewLSP(pathForBLSP, wavesB, waveLengthNetwork, "bp", id, protectionType);

                            isLSPestablish = true;
                            newLP++;
                        }
                    }

                    
                    
                }

                //Already there is path(LP) for LSP request
                else
                {  
                    if(pathDetails.tempCanCreatPP)
                    {
                        vector<int> waveVector = waveLengthNetwork.getWaveNumbers(source,destination,numOfNodes, bandwidth, pathDetails.tempPrimaryShortPath.size());
                        
                        if(waveVector[0] == -1)
                        {
                            //find a backup path
                            int backupWave = waveLengthNetwork.getBWaveNumber(source,destination,numOfNodes,bandwidth,pathDetails.tempPrimaryShortPath);
                            if(backupWave != -1)
                            {
                                myfile.writeLog("New lsp establish with new LP and old LP.");
                                waveLengthNetwork.setANewLighpath(pathDetails.tempPrimaryShortPath,pathDetails.tempWavelengthNoPP,"pp");
                                subWaveNetworks[pathDetails.tempWavelengthNoPP].removeLink(source,destination);

                                vector<int> pathForLSP = {source,destination};
                                vector<int> wavesP = {pathDetails.tempWavelengthNoPP};
                                vector<int> wavesB = {backupWave};

                                waveLengthNetwork.setANewLSP(pathForLSP, wavesP, waveLengthNetwork, "pp", id, protectionType);
                                waveLengthNetwork.setANewLSP(pathForLSP, wavesB, waveLengthNetwork, "bp", id, protectionType);

                                newLPnoldLP++;
                                isLSPestablish = true;
                            }
                        }
                        else
                        {
                            myfile.writeLog("New lsp established with 2 old LPs.");
                            vector<int> pathForLSP = {source,destination};
                            vector<int> wavesP = {waveVector[0]};
                            vector<int> wavesB = {waveVector[1]};

                            waveLengthNetwork.setANewLSP(pathForLSP,wavesP,waveLengthNetwork,"pp",id, protectionType);
                            waveLengthNetwork.setANewLSP(pathForLSP,wavesB,waveLengthNetwork,"bp",id, protectionType);
                            
                            oldLP++;
                            isLSPestablish = true;
                        }
                    }                
                }

                if(!isLSPestablish) 
                {
                    rejectedEvents.push_back(listOfEvents[0].identifier);
                    rejected++;
                    myfile.writeLog("LSP is REJECTED.");
                }
                
                
            }
            itr = listOfEvents.begin();
            listOfEvents.erase(itr);
        } 

        myfile.writeLog("                 ");
        myfile.writeLog("****Counts****");
        myfile.writeLog("Num of lsp rqst = "+to_string(numberOfLSPrequests));
        myfile.writeLog("new LP & old LP = "+to_string(newLPnoldLP));
        myfile.writeLog("new 2 LPs  = "+to_string(new2LPs));
        myfile.writeLog("new LP = "+to_string(newLP));
        myfile.writeLog("old LP = "+to_string(oldLP));
        myfile.writeLog("                 ");
        myfile.writeLog("**Num.of LSP established = "+to_string(newLPnoldLP+new2LPs+newLP+oldLP));
        myfile.writeLog("**Num.of LSP rejected = "+to_string(rejected));
    }

   
    
	cin.get();
    return 0;
}