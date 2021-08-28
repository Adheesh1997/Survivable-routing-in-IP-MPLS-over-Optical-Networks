#include <iostream>
#include <string>
#include <vector>
#include <numeric>

#include "files.h"
#include "graph.h"
#include "lightpathSettingup.h"
#include "Dijkstra.h"
#include "lspRequestGenarator.h"
#include "eventDrivenSimulator.h"

using namespace std;

//int _couter = 0;

void print2dVector(vector<vector<int>> adjMetrixForPrimaryLSP)
{
    for(vector<int> x:adjMetrixForPrimaryLSP)
    {
        for(int y:x)
        {
            cout<<y<<" ";
        }cout<<endl;
    }
}

void printVector(vector<int> arr)
{
    cout<<endl;
    for(int i:arr)
    {
        cout<<i<<"-> ";
    }
}

void checkWhetherLPidfinished(vector<int> &LPids, int &countForLPids)
{
    if (LPids.empty())
        LPids.push_back(++countForLPids);
}

void removeLink(int waveNum, vector<int> path, vector<waveLengthNetworks>& subWaveNetworks)
{
    //cout<<"\nWave : "<<waveNum<<"\nPaht ";
    //printVector(path);
    for (int i = 0; i < path.size() - 1; i++)
    {
        //cout<<"\nIn main"<<", Source : "<<path[i]<<" , Dst : "<<path[i+1]<<endl;
        subWaveNetworks[waveNum].removeLink(path[i], path[i + 1]);
    }
}

void removeLPidFromVec(vector<int>& LPids)
{
    vector<int>::iterator LPitr;
    LPitr = LPids.begin();
    LPids.erase(LPitr);
}

int main()
{

    cout << "Promgram started___\n";
    files myfile;
    int numOfNodes; //Variable to store number of nodes in the network

    int _couter = 0;

    thresholds thresholdObj;
    bool protectionType = false;              //True for bandwidth based LP protection. False for number of LSPs based LP protection
    thresholdObj.bandwidthThreshold = 0.2;  //Assigning the threshold values
    thresholdObj.numLSPthreshold = 1;        //Assigning the threshold values
    int numberOfLSPrequests = 1000;           //The number of LSP requests
    double erlang = 200;                      //Erlang value
    double meanHoldingTime = 1;              //Mean holding time
    int numOfWaves = 16;

    requestCreation tempObject;
    tempObject.requestGenerator(numberOfLSPrequests, erlang, meanHoldingTime);   //Create the LSP requests
    //tempObject.printLSPrequests();                                             //Print the LSP requests
    //
    //tempObject.printEvents();                                                  //Print the events
    vector<int> rejectedEvents;                                                  //To capture the rejected events
    vector<events>::iterator itr;

    // Only (1) or (2) keep uncomment at one time , Dont both or Dont keep both comment!!!!!

    /*************** Read event to a file*****************  -------------------------(1)  **/
    vector<events> listOfEvents = tempObject.eventCreation();                    //Create the events
    myfile.wrteALSP("rqst_inputs/rq5.txt", listOfEvents); 
    /*************** end of (1) *******************/

    /**************** LSP requests read from file ********** -------------------------(2) */
    //vector<events> listOfEvents;
    //myfile.readLSPs("rqst_inputs/rq4.txt", listOfEvents);
    /*************** end of (2) *******************/

    vector<vector<int>> adjacencyMetrix; //Vector to store adjacency metrix that represent netork

    map<int, map<int,vector<vector<int>>>> lspPathDetails;

    //graph input file location
    string fileLocation = "graph_inputs/05/graph05.csv"; 
    
    int theCount = 1;
    //Read csv file and assign values to the matrix 
    if(myfile.readGraphInputFile(numOfNodes, adjacencyMetrix,fileLocation,numOfWaves))
    { 
        myfile.writeLog((fileLocation + " Graph is imported."));
        cout << "Graph imported from csv file.\n";
        //If there is no any error while reading file then graph is created
        fiberLinkNetwork physicalLinkNetwork(numOfNodes, numOfWaves);
        thresholdObj.numOfNodesOfTheNetwork = numOfNodes;

        physicalLinkNetwork.setupFiberLinkNetwork(adjacencyMetrix,numOfWaves);
        cout << "Physical link network is created.\n";

        myfile.writeLog("Physical network is created.");
        //physicalLinkNetwork.printGraph();

        //Generate sub wavelength graphs for establish LPs
        vector<waveLengthNetworks> subWaveNetworks = setupWaveLengthNetworks(adjacencyMetrix, numOfWaves);

        waveLengthNetworks defaulSubWaveNetworks = subWaveNetworks[0];
        
        lightpathNetwork waveLengthNetwork(subWaveNetworks);
        
        
        cout << endl << "Generting all LSP requests.\n";
        lspRequestGenarator lspReqGen(numOfNodes);
        lspRequest lspReq;

        LSP lspObj;


        //New counters
        int rejected = 0;
        int pathNotFound = 0;

        int noAlreadyPathEstablish = 0;
        int noAlreadyPathReject = 0;
            int newLPnNewLP = 0;
            int newLPnCombineLP = 0;
            int combineLPnCombineLP = 0;


        int AlreadyPathEstablish = 0;
        int AlreadyPathReject = 0;
            int tempNAlready = 0;
            int tempNtemp = 0;
            int tempNcombine = 0;
            int oldNold = 0;
            int oldNnew = 0;
            int oldNcombine = 0;
        
        vector<int> LPids(100000); // vector with 100000 ints.
        iota(begin(LPids), end(LPids), 1); // Fill with 1, 2, ..., 100000.
        int countForLPids = 100000;
        
        cout << "Establishing and releasing LSP requests.....\n";
        vector<int> pPathFortest;
        vector<int> bPathFortest;
        vector<int> waveVecFortest;

        while(!listOfEvents.empty())//for(events event:tempObject.eventVector)
        //for(int pp = 0; pp < listOfEvents.size(); pp++)
        {
            
            int vexnum = numOfNodes;
            int source = listOfEvents[0].sourceNode;
            int destination = listOfEvents[0].destinationNode;
            int id = listOfEvents[0].identifier;
            int bandwidth = listOfEvents[0].bandwidth;
            bool action = listOfEvents[0].action;
            
            //Generte a lsp reqest with src,dst,bandwidth, request or remove
            bool isLSPestablish = false;            
            /* for (int ww = 0; ww < rejectedEvents.size(); ww++)
            {
                if (listOfEvents[0].identifier == rejectedEvents[ww])
                {
                    itr = listOfEvents.begin();
                    listOfEvents.erase(itr);

                    vector<int>::iterator itr2;
                    itr2 = rejectedEvents.begin() + ww;
                    rejectedEvents.erase(itr2);
                }
            } */


            lspPathDetails[id][0].push_back(vector<int>{-1});
            lspPathDetails[id][0].push_back(vector<int>{-1});
            lspPathDetails[id][1].push_back(vector<int>{-1});
            lspPathDetails[id][1].push_back(vector<int>{-1});
            
            if(action)
            {   
                
                cout <<"\nRqst : " << id;
                myfile.writeLog(("New request. Bandwidth = "+to_string(bandwidth)+",source = "+to_string(source)+", Dst = "
                                +to_string(destination)+", id = "+to_string(id)+", request = "+to_string(action)));

                vector<vector<int>> adjMetrixForPrimaryLSP = waveLengthNetwork.lpPAdjacencyMetrix(bandwidth, numOfNodes);
                
                findPathDetails pathDetails = startingPoint(vexnum, subWaveNetworks, source, destination,adjMetrixForPrimaryLSP);

                //map<int, map<int, vector<vector<int>>>> mapFromLPGraph;

                //If there is no LP(S) to establish LSP
                if (pathDetails.alreadyPPhave == false) 
                {
                    
                    if(pathDetails.canCreatBP && pathDetails.canCreatPP && !isLSPestablish)
                    {
                        checkWhetherLPidfinished(LPids, countForLPids);
                        waveLengthNetwork.setANewLighpath(pathDetails.primaryShortPath, pathDetails.wavelengthNoPP,"pp", LPids[0]);
                        removeLink(pathDetails.wavelengthNoPP, pathDetails.primaryShortPath, subWaveNetworks);
                        removeLPidFromVec(LPids);

                        checkWhetherLPidfinished(LPids, countForLPids);
                        waveLengthNetwork.setANewLighpath(pathDetails.backUpShortPath,pathDetails.wavelengthNoBP,"pp", LPids[0]);
                        removeLink(pathDetails.wavelengthNoBP, pathDetails.backUpShortPath, subWaveNetworks);
                        removeLPidFromVec(LPids);

                        vector<int> wholePathP = pathDetails.primaryShortPath;
                        vector<int> pathForPLSP = {pathDetails.primaryShortPath[0],pathDetails.primaryShortPath[pathDetails.primaryShortPath.size()-1]};
                        vector<int> wavesP = {pathDetails.wavelengthNoPP};
                        lspObj.makeLSP(bandwidth, pathForPLSP, wholePathP, wavesP, waveLengthNetwork, "pLSP", id, protectionType, thresholdObj);
                        
                        vector<int> wholePathB = pathDetails.backUpShortPath;
                        vector<int> pathForBLSP = {pathDetails.backUpShortPath[0],pathDetails.backUpShortPath[pathDetails.backUpShortPath.size()-1]};
                        vector<int> wavesB = {pathDetails.wavelengthNoBP};
                        lspObj.makeLSP(bandwidth, pathForBLSP, wholePathB, wavesB, waveLengthNetwork, "bLSP", id, protectionType, thresholdObj);
                        
                        lspPathDetails[id][0][0] = pathForPLSP;
                        lspPathDetails[id][0][1] = wavesP;
                        lspPathDetails[id][1][0] = pathForBLSP;
                        lspPathDetails[id][1][1] = wavesB;
                        
                        myfile.writeLog("New lsp established with 2 new LPs. ["+to_string(pathDetails.wavelengthNoPP)+"] ["+to_string(pathDetails.wavelengthNoBP)+"]");
                        
                        isLSPestablish = true;
                        newLPnNewLP++;
                                                   
                    }

                    map<int,vector<vector<int>>> arr = waveLengthNetwork.mapFromsource(source,numOfNodes, bandwidth,numOfWaves);
                    map<int,vector<vector<int>>> arr1 = waveLengthNetwork.mapFromsource(destination,numOfNodes, bandwidth,numOfWaves);
                    
                    //If LP can not creat for primary path
                    if(pathDetails.canCreatPP && !pathDetails.canCreatBP && !isLSPestablish)
                    {
                        forRemainingBackUpPath createRemaingBackUpDeatils = createRemaing(vexnum, subWaveNetworks, source, destination, arr, arr1, pathDetails.primaryShortPath);

                        if(createRemaingBackUpDeatils.canCreatCombinationBP)
                        {
                            
                            if(createRemaingBackUpDeatils.wavelengthNo1BP < numOfWaves)
                            {
                                checkWhetherLPidfinished(LPids, countForLPids);
                                waveLengthNetwork.setANewLighpath(createRemaingBackUpDeatils.w1ShortPathBP,createRemaingBackUpDeatils.wavelengthNo1BP,"pp", LPids[0]);
                                removeLink(createRemaingBackUpDeatils.wavelengthNo1BP, createRemaingBackUpDeatils.w1ShortPathBP, subWaveNetworks);
                                removeLPidFromVec(LPids);
                            }
                            else
                            {
                                createRemaingBackUpDeatils.wavelengthNo1BP -= numOfWaves;
                            }

                            if(createRemaingBackUpDeatils.wavelengthNo2BP < numOfWaves)
                            {
                                checkWhetherLPidfinished(LPids, countForLPids);
                                waveLengthNetwork.setANewLighpath(createRemaingBackUpDeatils.w2ShortPathBP,createRemaingBackUpDeatils.wavelengthNo2BP,"pp", LPids[0]);
                                removeLink(createRemaingBackUpDeatils.wavelengthNo2BP, createRemaingBackUpDeatils.w2ShortPathBP, subWaveNetworks);
                                removeLPidFromVec(LPids);
                            }
                            else
                            {
                                createRemaingBackUpDeatils.wavelengthNo2BP -= numOfWaves;
                            }

                            checkWhetherLPidfinished(LPids, countForLPids);
                            waveLengthNetwork.setANewLighpath(pathDetails.primaryShortPath, pathDetails.wavelengthNoPP,"pp", LPids[0]);
                            removeLink(pathDetails.wavelengthNoPP, pathDetails.primaryShortPath, subWaveNetworks);
                            removeLPidFromVec(LPids);

                            vector<int>wholePathP = pathDetails.primaryShortPath;
                            vector<int> pathForPLSP = {pathDetails.primaryShortPath[0],pathDetails.primaryShortPath[pathDetails.primaryShortPath.size()-1]};
                            vector<int> wavesP = {pathDetails.wavelengthNoPP};
                            
                            vector<int> wholePathB = createRemaingBackUpDeatils.w1ShortPathBP;
                            wholePathB.insert(wholePathB.end(), createRemaingBackUpDeatils.w2ShortPathBP.begin() + 1, createRemaingBackUpDeatils.w2ShortPathBP.end());

                            vector<int> pathForBLSP = {source, createRemaingBackUpDeatils.connectingNodeBP, destination};
                            vector<int> wavesB = {createRemaingBackUpDeatils.wavelengthNo1BP, createRemaingBackUpDeatils.wavelengthNo2BP};

                            lspObj.makeLSP(bandwidth, pathForBLSP, wholePathB, wavesB, waveLengthNetwork, "bLSP", id, protectionType, thresholdObj, vector<vector<int>>{ createRemaingBackUpDeatils.w1ShortPathBP, createRemaingBackUpDeatils.w2ShortPathBP});
                            lspObj.makeLSP(bandwidth, pathForPLSP, wholePathP, wavesP, waveLengthNetwork, "pLSP", id, protectionType, thresholdObj);

                            myfile.writeLog("New LSP establish with single LP for primary, and 2 combine LPs for backup. ["+to_string(wavesP[0])+"] ["+to_string(wavesB[0])+","+to_string(wavesB[1])+"]");
                            
                            lspPathDetails[id][0][0] = pathForPLSP;
                            lspPathDetails[id][0][1] = wavesP;
                            lspPathDetails[id][1][0] = pathForBLSP;
                            lspPathDetails[id][1][1] = wavesB;

                            isLSPestablish = true;
                            newLPnCombineLP++;

                        }
                    }
                                     
                    if(!pathDetails.canCreatPP && !pathDetails.canCreatBP && !isLSPestablish)
                    {
                        combineWavelength combineWavelengthDetails = pathCombinationCreat(vexnum, subWaveNetworks, source, destination, arr, arr1);
                        
                        if(combineWavelengthDetails.canCreatCombinationPP && combineWavelengthDetails.canCreatCombinationBP)
                        {
                            if (combineWavelengthDetails.wavelengthNo1PP < numOfWaves)
                            {
                                checkWhetherLPidfinished(LPids, countForLPids);
                                waveLengthNetwork.setANewLighpath(combineWavelengthDetails.w1ShortPathPP, combineWavelengthDetails.wavelengthNo1PP, "pp", LPids[0]);
                                removeLink(combineWavelengthDetails.wavelengthNo1PP, combineWavelengthDetails.w1ShortPathPP, subWaveNetworks);
                                removeLPidFromVec(LPids);

                            }
                            else combineWavelengthDetails.wavelengthNo1PP -= numOfWaves;

                            if (combineWavelengthDetails.wavelengthNo2PP < numOfWaves)
                            {
                                checkWhetherLPidfinished(LPids, countForLPids);
                                waveLengthNetwork.setANewLighpath(combineWavelengthDetails.w2ShortPathPP, combineWavelengthDetails.wavelengthNo2PP, "pp", LPids[0]);
                                removeLink(combineWavelengthDetails.wavelengthNo2PP, combineWavelengthDetails.w2ShortPathPP, subWaveNetworks);
                                removeLPidFromVec(LPids);

                            }
                            else combineWavelengthDetails.wavelengthNo2PP -= numOfWaves;


                            if (combineWavelengthDetails.wavelengthNo1BP < numOfWaves)
                            {
                                checkWhetherLPidfinished(LPids, countForLPids);
                                waveLengthNetwork.setANewLighpath(combineWavelengthDetails.w1ShortPathBP, combineWavelengthDetails.wavelengthNo1BP, "pp", LPids[0]);
                                removeLink(combineWavelengthDetails.wavelengthNo1BP, combineWavelengthDetails.w1ShortPathBP, subWaveNetworks);
                                removeLPidFromVec(LPids);

                            }
                            
                            else combineWavelengthDetails.wavelengthNo1BP -= numOfWaves;

                            if (combineWavelengthDetails.wavelengthNo2BP < numOfWaves)
                            {
                                checkWhetherLPidfinished(LPids, countForLPids);
                                waveLengthNetwork.setANewLighpath(combineWavelengthDetails.w2ShortPathBP, combineWavelengthDetails.wavelengthNo2BP, "pp", LPids[0]);
                                removeLink(combineWavelengthDetails.wavelengthNo2BP, combineWavelengthDetails.w2ShortPathBP, subWaveNetworks);
                                removeLPidFromVec(LPids);

                            }
                            
                            else combineWavelengthDetails.wavelengthNo2BP -= numOfWaves;

                            
                            vector<int> wholePathP = combineWavelengthDetails.w1ShortPathPP;
                            wholePathP.insert(wholePathP.end(), combineWavelengthDetails.w2ShortPathPP.begin() + 1, combineWavelengthDetails.w2ShortPathPP.end());
                            vector<int> pathForPLSP = {source, combineWavelengthDetails.connectingNodePP, destination};
                            vector<int> wavesP = {combineWavelengthDetails.wavelengthNo1PP, combineWavelengthDetails.wavelengthNo2PP};
                            
                            lspObj.makeLSP(bandwidth, pathForPLSP, wholePathP, wavesP, waveLengthNetwork, "pLSP", id, protectionType, thresholdObj, vector<vector<int>>{combineWavelengthDetails.w1ShortPathPP, combineWavelengthDetails.w2ShortPathPP});

                            vector<int> wholePathB = combineWavelengthDetails.w1ShortPathBP;
                            wholePathB.insert(wholePathB.end(), combineWavelengthDetails.w2ShortPathBP.begin() + 1, combineWavelengthDetails.w2ShortPathBP.end());
                            vector<int> pathForBLSP = {source, combineWavelengthDetails.connectingNodeBP, destination};
                            vector<int> wavesB = {combineWavelengthDetails.wavelengthNo1BP, combineWavelengthDetails.wavelengthNo2BP};

                            lspObj.makeLSP(bandwidth, pathForBLSP, wholePathB, wavesB, waveLengthNetwork, "bLSP", id, protectionType, thresholdObj, vector<vector<int>>{combineWavelengthDetails.w1ShortPathBP, combineWavelengthDetails.w2ShortPathBP});

                            myfile.writeLog("New LSP establish with combine LPs for both primary and backup LSPs. ["
                                            +to_string(wavesP[0])+","+to_string(wavesP[1])+"] ["+to_string(wavesB[0])+","+to_string(wavesB[1])+"]");
                            
                            lspPathDetails[id][0][0] = pathForPLSP;
                            lspPathDetails[id][0][1] = wavesP;
                            lspPathDetails[id][1][0] = pathForBLSP;
                            lspPathDetails[id][1][1] = wavesB;

                            isLSPestablish = true;
                            combineLPnCombineLP++;
                            
                        }
                    }

                    if (!isLSPestablish)
                    {
                        map<int, map<int, vector<vector<int>>>> mapFromLPGraph = waveLengthNetwork.mapFromLpGraph(numOfWaves, numOfNodes, bandwidth);

                        moreOEOConvertion moreCovertions = createMainGraph(vexnum, subWaveNetworks, source, destination, mapFromLPGraph);

                        if (moreCovertions.canCreateBP && moreCovertions.canCreatePP)
                        {

                            // Creating LPs for primary and backup paths
                            for (int primaryPart = 0; primaryPart < moreCovertions.wavelengthNumberPP.size(); primaryPart++)
                            {
                                if (moreCovertions.wavelengthNumberPP[primaryPart] < numOfWaves)
                                {
                                    checkWhetherLPidfinished(LPids, countForLPids);
                                    waveLengthNetwork.setANewLighpath(moreCovertions.allPathDetailsPP[primaryPart], moreCovertions.wavelengthNumberPP[primaryPart], "pp", LPids[0]);
                                    removeLink(moreCovertions.wavelengthNumberPP[primaryPart], moreCovertions.allPathDetailsPP[primaryPart], subWaveNetworks);
                                    removeLPidFromVec(LPids);
                                }
                                else
                                {
                                    moreCovertions.wavelengthNumberPP[primaryPart] -= numOfWaves;
                                }
                            }

                            for (int backupPart = 0; backupPart < moreCovertions.wavelengthNumberBP.size(); backupPart++)
                            {
                                if (moreCovertions.wavelengthNumberBP[backupPart] < numOfWaves)
                                {
                                    checkWhetherLPidfinished(LPids, countForLPids);
                                    waveLengthNetwork.setANewLighpath(moreCovertions.allPathDetailsBP[backupPart], moreCovertions.wavelengthNumberBP[backupPart], "pp", LPids[0]);
                                    removeLink(moreCovertions.wavelengthNumberBP[backupPart], moreCovertions.allPathDetailsBP[backupPart], subWaveNetworks);
                                    removeLPidFromVec(LPids);
                                }
                                else
                                {
                                    moreCovertions.wavelengthNumberBP[backupPart] -= numOfWaves;
                                }
                            }


                            // Creating LSPs for primary and backup
                            vector<int> wholePathP;
                            vector<int> pathForPLSP = { moreCovertions.allPathDetailsPP[0][0] };
                            vector<int> wavesP = moreCovertions.wavelengthNumberPP;

                            for (int primaryPart = 0; primaryPart < moreCovertions.allPathDetailsPP.size(); primaryPart++)
                            {
                                wholePathP.insert(wholePathP.end(), moreCovertions.allPathDetailsPP[primaryPart].begin(), moreCovertions.allPathDetailsPP[primaryPart].end());
                                pathForPLSP.push_back(moreCovertions.allPathDetailsPP[primaryPart].back());
                               
                            }

                            vector<int> wholePathB;
                            vector<int> pathForBLSP = { moreCovertions.allPathDetailsBP[0][0] };
                            vector<int> wavesB = moreCovertions.wavelengthNumberBP;

                            for (int backupPart = 0; backupPart < moreCovertions.allPathDetailsBP.size(); backupPart++)
                            {
                                wholePathB.insert(wholePathB.end(), moreCovertions.allPathDetailsBP[backupPart].begin(), moreCovertions.allPathDetailsBP[backupPart].end());
                                pathForBLSP.push_back(moreCovertions.allPathDetailsBP[backupPart].back());
                                
                            }

                            lspObj.makeLSP(bandwidth, pathForPLSP, wholePathP, wavesP, waveLengthNetwork, "pLSP", id, protectionType, thresholdObj, moreCovertions.allPathDetailsPP);
                            lspObj.makeLSP(bandwidth, pathForBLSP, wholePathB, wavesB, waveLengthNetwork, "bLSP", id, protectionType, thresholdObj, moreCovertions.allPathDetailsBP);

                            isLSPestablish = true;
                            combineLPnCombineLP++;

                            lspPathDetails[id][0][0] = pathForPLSP;
                            lspPathDetails[id][0][1] = wavesP;
                            lspPathDetails[id][1][0] = pathForBLSP;
                            lspPathDetails[id][1][1] = wavesB;

                            
                        }
                    }
                    
                        
                    if (isLSPestablish) noAlreadyPathEstablish++;
                    else noAlreadyPathReject++;
                }
                else
                { 
                    vector<vector<int>>  waveVector = waveLengthNetwork.getWaveNumbers(source, destination, numOfNodes, bandwidth, 14);

                    if (waveVector[0][0] != -1)
                    {
                        if (pathDetails.tempCanCreatPP)
                        {
                            waveVector = waveLengthNetwork.getWaveNumbers(source, destination, numOfNodes, bandwidth, pathDetails.tempPrimaryShortPath.size());

                            if (waveVector[0][0] == -1)
                            {
                                vector<int> bWaveVector = waveLengthNetwork.getBWaveNumber(source, destination, numOfNodes, bandwidth, pathDetails.tempPrimaryShortPath);

                                if (bWaveVector[0] != -1)
                                {
                                    checkWhetherLPidfinished(LPids, countForLPids);
                                    waveLengthNetwork.setANewLighpath(pathDetails.tempPrimaryShortPath, pathDetails.tempWavelengthNoPP, "pp", LPids[0]);
                                    removeLink(pathDetails.tempWavelengthNoPP, pathDetails.tempPrimaryShortPath, subWaveNetworks);
                                    removeLPidFromVec(LPids);

                                    vector<int> wholePathP = pathDetails.tempPrimaryShortPath;
                                    vector<int> pathForLSP = { pathDetails.tempPrimaryShortPath[0],pathDetails.tempPrimaryShortPath.back() };
                                    vector<int> wavesP = { pathDetails.tempWavelengthNoPP };

                                    vector<int> wholePathB = bWaveVector;
                                    wholePathB.erase(wholePathB.begin());
                                    vector<int> wavesB = { bWaveVector[0] };

                                    lspObj.makeLSP(bandwidth, pathForLSP, wholePathP, wavesP, waveLengthNetwork, "pLSP", id, protectionType, thresholdObj);
                                    lspObj.makeLSP(bandwidth, pathForLSP, wholePathB, wavesB, waveLengthNetwork, "bLSP", id, protectionType, thresholdObj);
                                    
                                    lspPathDetails[id][0][0] = pathForLSP;
                                    lspPathDetails[id][0][1] = wavesP;
                                    lspPathDetails[id][1][0] = pathForLSP;
                                    lspPathDetails[id][1][1] = wavesB;

                                    isLSPestablish = true; 
                                    tempNAlready++;
                                }
                                else
                                {

                                    if (pathDetails.tempCanCreatBP)
                                    {
                                        checkWhetherLPidfinished(LPids, countForLPids);
                                        waveLengthNetwork.setANewLighpath(pathDetails.tempPrimaryShortPath, pathDetails.tempWavelengthNoPP, "pp", LPids[0]);
                                        removeLink(pathDetails.tempWavelengthNoPP, pathDetails.tempPrimaryShortPath, subWaveNetworks);
                                        removeLPidFromVec(LPids);

                                        checkWhetherLPidfinished(LPids, countForLPids);
                                        waveLengthNetwork.setANewLighpath(pathDetails.tempBackUpShortPath, pathDetails.tempWavelengthNoBP, "pp", LPids[0]);
                                        removeLink(pathDetails.tempWavelengthNoBP, pathDetails.tempBackUpShortPath, subWaveNetworks);
                                        removeLPidFromVec(LPids);


                                        //subWaveNetworks[pathDetails.tempWavelengthNoPP].removeLink(source,destination);

                                        vector<int> wholePathP = pathDetails.tempPrimaryShortPath;
                                        vector<int> wholePathB = pathDetails.tempBackUpShortPath;


                                        vector<int> pathForLSP = { source,destination };
                                        vector<int> wavesP = { pathDetails.tempWavelengthNoPP };
                                        vector<int> wavesB = { pathDetails.tempWavelengthNoBP };

                                        lspObj.makeLSP(bandwidth, pathForLSP, wholePathP, wavesP, waveLengthNetwork, "pLSP", id, protectionType, thresholdObj);
                                        lspObj.makeLSP(bandwidth, pathForLSP, wholePathB, wavesB, waveLengthNetwork, "bLSP", id, protectionType, thresholdObj);

                                        lspPathDetails[id][0][0] = pathForLSP;
                                        lspPathDetails[id][0][1] = wavesP;
                                        lspPathDetails[id][1][0] = pathForLSP;
                                        lspPathDetails[id][1][1] = wavesB;

                                        myfile.writeLog("New lsp establish with new LP and old LP. [" + to_string(wavesP[0]) + "] [" + to_string(wavesB[0]) + "]");
                                        
                                        isLSPestablish = true;
                                        tempNtemp++;

                                    }
                                    else
                                    {
                                        map<int, vector<vector<int>>> arr = waveLengthNetwork.mapFromsource(source, numOfNodes, bandwidth, numOfWaves);
                                        map<int, vector<vector<int>>> arr1 = waveLengthNetwork.mapFromsource(destination, numOfNodes, bandwidth, numOfWaves);

                                        forRemainingBackUpPath createRemaingBackUpDeatils = createRemaing(vexnum, subWaveNetworks, source, destination, arr, arr1, pathDetails.tempPrimaryShortPath);

                                        if (createRemaingBackUpDeatils.canCreatCombinationBP)
                                        {

                                            if (createRemaingBackUpDeatils.wavelengthNo1BP < numOfWaves)
                                            {
                                                checkWhetherLPidfinished(LPids, countForLPids);
                                                waveLengthNetwork.setANewLighpath(createRemaingBackUpDeatils.w1ShortPathBP, createRemaingBackUpDeatils.wavelengthNo1BP, "pp", LPids[0]);
                                                removeLink(createRemaingBackUpDeatils.wavelengthNo1BP, createRemaingBackUpDeatils.w1ShortPathBP, subWaveNetworks);
                                                removeLPidFromVec(LPids);
                                            }
                                            else
                                            {
                                                createRemaingBackUpDeatils.wavelengthNo1BP -= numOfWaves;
                                            }

                                            if (createRemaingBackUpDeatils.wavelengthNo2BP < numOfWaves)
                                            {
                                                checkWhetherLPidfinished(LPids, countForLPids);
                                                waveLengthNetwork.setANewLighpath(createRemaingBackUpDeatils.w2ShortPathBP, createRemaingBackUpDeatils.wavelengthNo2BP, "pp", LPids[0]);
                                                removeLink(createRemaingBackUpDeatils.wavelengthNo2BP, createRemaingBackUpDeatils.w2ShortPathBP, subWaveNetworks);
                                                removeLPidFromVec(LPids);
                                            }
                                            else
                                            {
                                                createRemaingBackUpDeatils.wavelengthNo2BP -= numOfWaves;
                                            }

                                            checkWhetherLPidfinished(LPids, countForLPids);
                                            waveLengthNetwork.setANewLighpath(pathDetails.tempPrimaryShortPath, pathDetails.tempWavelengthNoPP, "pp", LPids[0]);
                                            removeLink(pathDetails.tempWavelengthNoPP, pathDetails.tempPrimaryShortPath, subWaveNetworks);
                                            removeLPidFromVec(LPids);

                                            vector<int>wholePathP = pathDetails.tempPrimaryShortPath; 
                                            vector<int> pathForPLSP = { source, destination};
                                            vector<int> wavesP = { pathDetails.tempWavelengthNoPP };

                                            vector<int> wholePathB = createRemaingBackUpDeatils.w1ShortPathBP;
                                            wholePathB.insert(wholePathB.end(), createRemaingBackUpDeatils.w2ShortPathBP.begin() + 1, createRemaingBackUpDeatils.w2ShortPathBP.end());

                                            vector<int> pathForBLSP = { source, createRemaingBackUpDeatils.w2ShortPathBP[0], destination };
                                            vector<int> wavesB = { createRemaingBackUpDeatils.wavelengthNo1BP, createRemaingBackUpDeatils.wavelengthNo2BP };

                                            lspObj.makeLSP(bandwidth, pathForBLSP, wholePathB, wavesB, waveLengthNetwork, "bLSP", id, protectionType, thresholdObj, vector<vector<int>>{ createRemaingBackUpDeatils.w1ShortPathBP, createRemaingBackUpDeatils.w2ShortPathBP});
                                            lspObj.makeLSP(bandwidth, pathForPLSP, wholePathP, wavesP, waveLengthNetwork, "pLSP", id, protectionType, thresholdObj);

                                            myfile.writeLog("New LSP establish with single LP for primary, and 2 combine LPs for backup. [" + to_string(wavesP[0]) + "] [" + to_string(wavesB[0]) + "," + to_string(wavesB[1]) + "]");

                                            lspPathDetails[id][0][0] = pathForPLSP;
                                            lspPathDetails[id][0][1] = wavesP;
                                            lspPathDetails[id][1][0] = pathForBLSP;
                                            lspPathDetails[id][1][1] = wavesB;

                                            isLSPestablish = true;
                                            tempNcombine++;

                                        }
                                    }
                                }
                                
                            }
                            else
                            {
                                if (waveVector[1][0] != -1)
                                {
                                    vector<int> wholePathP = waveVector[0];
                                    wholePathP.erase(wholePathP.begin());

                                    vector<int> wholePathB = waveVector[1];
                                    wholePathB.erase(wholePathB.begin());

                                    vector<int> pathForLSP = { source,destination };
                                    vector<int> wavesP = { waveVector[0][0] };
                                    vector<int> wavesB = { waveVector[1][0] };

                                    lspObj.makeLSP(bandwidth, pathForLSP, wholePathP, wavesP, waveLengthNetwork, "pLSP", id, protectionType, thresholdObj);
                                    lspObj.makeLSP(bandwidth, pathForLSP, wholePathB, wavesB, waveLengthNetwork, "bLSP", id, protectionType, thresholdObj);

                                    lspPathDetails[id][0][0] = pathForLSP;
                                    lspPathDetails[id][0][1] = wavesP;
                                    lspPathDetails[id][1][0] = pathForLSP;
                                    lspPathDetails[id][1][1] = wavesB;

                                    myfile.writeLog("New lsp established with 2 old LPs. [" + to_string(wavesP[0]) + "] [" + to_string(wavesB[0]) + "]");
                                    
                                    isLSPestablish = true;
                                    oldNold++;

                                }
                                else if (pathDetails.tempCanCreatBP)
                                {
                                    checkWhetherLPidfinished(LPids, countForLPids);
                                    waveLengthNetwork.setANewLighpath(pathDetails.tempBackUpShortPath, pathDetails.tempWavelengthNoBP, "pp", LPids[0]);
                                    removeLink(pathDetails.tempWavelengthNoBP, pathDetails.tempBackUpShortPath, subWaveNetworks);
                                    removeLPidFromVec(LPids);

                                    vector<int>wholePathP = waveVector[0];
                                    wholePathP.erase(wholePathP.begin());
                                    vector<int> pathForPLSP = { source, destination };
                                    vector<int> wavesP = { waveVector[0][0] };

                                    vector<int>wholePathB = pathDetails.tempBackUpShortPath;
                                    vector<int> pathForBLSP = { source, destination };
                                    vector<int> wavesB = { pathDetails.tempWavelengthNoBP };

                                    lspObj.makeLSP(bandwidth, pathForPLSP, wholePathP, wavesP, waveLengthNetwork, "pLSP", id, protectionType, thresholdObj);
                                    lspObj.makeLSP(bandwidth, pathForBLSP, wholePathB, wavesB, waveLengthNetwork, "bLSP", id, protectionType, thresholdObj);

                                    lspPathDetails[id][0][0] = pathForPLSP;
                                    lspPathDetails[id][0][1] = wavesP;
                                    lspPathDetails[id][1][0] = pathForBLSP;
                                    lspPathDetails[id][1][1] = wavesB;

                                    isLSPestablish = true;
                                    oldNnew++;

                                }
                                else
                                {
                                    //waveVector = waveLengthNetwork.getWaveNumbers(source, destination, numOfNodes, bandwidth,14);

                                    map<int, vector<vector<int>>> arr = waveLengthNetwork.mapFromsource(source, numOfNodes, bandwidth, numOfWaves);
                                    map<int, vector<vector<int>>> arr1 = waveLengthNetwork.mapFromsource(destination, numOfNodes, bandwidth, numOfWaves);

                                    vector<int> primaryPath = waveVector[0];
                                    primaryPath.erase(primaryPath.begin());

                                    forRemainingBackUpPath createRemaingBackUpDeatils = createRemaing(vexnum, subWaveNetworks, source, destination, arr, arr1, primaryPath);

                                    if (createRemaingBackUpDeatils.canCreatCombinationBP)
                                    {

                                        if (createRemaingBackUpDeatils.wavelengthNo1BP < numOfWaves)
                                        {
                                            checkWhetherLPidfinished(LPids, countForLPids);
                                            waveLengthNetwork.setANewLighpath(createRemaingBackUpDeatils.w1ShortPathBP, createRemaingBackUpDeatils.wavelengthNo1BP, "pp", LPids[0]);
                                            removeLink(createRemaingBackUpDeatils.wavelengthNo1BP, createRemaingBackUpDeatils.w1ShortPathBP, subWaveNetworks);
                                            removeLPidFromVec(LPids);
                                        }
                                        else
                                        {
                                            createRemaingBackUpDeatils.wavelengthNo1BP -= numOfWaves;
                                        }

                                        if (createRemaingBackUpDeatils.wavelengthNo2BP < numOfWaves)
                                        {
                                            checkWhetherLPidfinished(LPids, countForLPids);
                                            waveLengthNetwork.setANewLighpath(createRemaingBackUpDeatils.w2ShortPathBP, createRemaingBackUpDeatils.wavelengthNo2BP, "pp", LPids[0]);
                                            removeLink(createRemaingBackUpDeatils.wavelengthNo2BP, createRemaingBackUpDeatils.w2ShortPathBP, subWaveNetworks);
                                            removeLPidFromVec(LPids);
                                        }
                                        else
                                        {
                                            createRemaingBackUpDeatils.wavelengthNo2BP -= numOfWaves;
                                        }


                                        vector<int>wholePathP = primaryPath;
                                        vector<int> pathForPLSP = {source, destination};
                                        vector<int> wavesP = { waveVector[0][0] };

                                        vector<int> wholePathB = createRemaingBackUpDeatils.w1ShortPathBP;
                                        wholePathB.insert(wholePathB.end(), createRemaingBackUpDeatils.w2ShortPathBP.begin() + 1, createRemaingBackUpDeatils.w2ShortPathBP.end());

                                        vector<int> pathForBLSP = { source, createRemaingBackUpDeatils.w2ShortPathBP[0], destination };
                                        vector<int> wavesB = { createRemaingBackUpDeatils.wavelengthNo1BP, createRemaingBackUpDeatils.wavelengthNo2BP };

                                        lspObj.makeLSP(bandwidth, pathForBLSP, wholePathB, wavesB, waveLengthNetwork, "bLSP", id, protectionType, thresholdObj, vector<vector<int>>{ createRemaingBackUpDeatils.w1ShortPathBP, createRemaingBackUpDeatils.w2ShortPathBP});
                                        lspObj.makeLSP(bandwidth, pathForPLSP, wholePathP, wavesP, waveLengthNetwork, "pLSP", id, protectionType, thresholdObj);

                                        myfile.writeLog("New LSP establish with single LP for primary, and 2 combine LPs for backup. [" + to_string(wavesP[0]) + "] [" + to_string(wavesB[0]) + "," + to_string(wavesB[1]) + "]");

                                        lspPathDetails[id][0][0] = pathForPLSP;
                                        lspPathDetails[id][0][1] = wavesP;
                                        lspPathDetails[id][1][0] = pathForBLSP;
                                        lspPathDetails[id][1][1] = wavesB;

                                        isLSPestablish = true;
                                        oldNcombine++;

                                    }
                                }

                            }

                        }
                        else
                        {
                            if (waveVector[1][0] != -1)
                            {
                                vector<int> wholePathP = waveVector[0];
                                wholePathP.erase(wholePathP.begin());

                                vector<int> wholePathB = waveVector[1];
                                wholePathB.erase(wholePathB.begin());

                                vector<int> pathForLSP = { source,destination };
                                vector<int> wavesP = { waveVector[0][0] };
                                vector<int> wavesB = { waveVector[1][0] };

                                lspObj.makeLSP(bandwidth, pathForLSP, wholePathP, wavesP, waveLengthNetwork, "pLSP", id, protectionType, thresholdObj);
                                lspObj.makeLSP(bandwidth, pathForLSP, wholePathB, wavesB, waveLengthNetwork, "bLSP", id, protectionType, thresholdObj);

                                lspPathDetails[id][0][0] = pathForLSP;
                                lspPathDetails[id][0][1] = wavesP;
                                lspPathDetails[id][1][0] = pathForLSP;
                                lspPathDetails[id][1][1] = wavesB;

                                myfile.writeLog("New lsp established with 2 old LPs. [" + to_string(wavesP[0]) + "] [" + to_string(wavesB[0]) + "]");
                                
                                isLSPestablish = true;
                                oldNold++;
                            }
                            else
                            {
                                map<int, vector<vector<int>>> arr = waveLengthNetwork.mapFromsource(source, numOfNodes, bandwidth, numOfWaves);
                                map<int, vector<vector<int>>> arr1 = waveLengthNetwork.mapFromsource(destination, numOfNodes, bandwidth, numOfWaves);

                                vector<int> primaryPath = waveVector[0];
                                primaryPath.erase(primaryPath.begin());

                                forRemainingBackUpPath createRemaingBackUpDeatils = createRemaing(vexnum, subWaveNetworks, source, destination, arr, arr1, primaryPath);

                                if (createRemaingBackUpDeatils.canCreatCombinationBP)
                                {

                                    if (createRemaingBackUpDeatils.wavelengthNo1BP < numOfWaves)
                                    {
                                        checkWhetherLPidfinished(LPids, countForLPids);
                                        waveLengthNetwork.setANewLighpath(createRemaingBackUpDeatils.w1ShortPathBP, createRemaingBackUpDeatils.wavelengthNo1BP, "pp", LPids[0]);
                                        removeLink(createRemaingBackUpDeatils.wavelengthNo1BP, createRemaingBackUpDeatils.w1ShortPathBP, subWaveNetworks);
                                        removeLPidFromVec(LPids);
                                    }
                                    else
                                    {
                                        createRemaingBackUpDeatils.wavelengthNo1BP -= numOfWaves;
                                    }

                                    if (createRemaingBackUpDeatils.wavelengthNo2BP < numOfWaves)
                                    {
                                        checkWhetherLPidfinished(LPids, countForLPids);
                                        waveLengthNetwork.setANewLighpath(createRemaingBackUpDeatils.w2ShortPathBP, createRemaingBackUpDeatils.wavelengthNo2BP, "pp", LPids[0]);
                                        removeLink(createRemaingBackUpDeatils.wavelengthNo2BP, createRemaingBackUpDeatils.w2ShortPathBP, subWaveNetworks);
                                        removeLPidFromVec(LPids);
                                    }
                                    else
                                    {
                                        createRemaingBackUpDeatils.wavelengthNo2BP -= numOfWaves;
                                    }


                                    vector<int>wholePathP = primaryPath;
                                    vector<int> pathForPLSP = { source, destination };
                                    vector<int> wavesP = { waveVector[0][0] };

                                    vector<int> wholePathB = createRemaingBackUpDeatils.w1ShortPathBP;
                                    wholePathB.insert(wholePathB.end(), createRemaingBackUpDeatils.w2ShortPathBP.begin() + 1, createRemaingBackUpDeatils.w2ShortPathBP.end());

                                    vector<int> pathForBLSP = { source, createRemaingBackUpDeatils.w2ShortPathBP[0], destination };
                                    vector<int> wavesB = { createRemaingBackUpDeatils.wavelengthNo1BP, createRemaingBackUpDeatils.wavelengthNo2BP };

                                    lspObj.makeLSP(bandwidth, pathForBLSP, wholePathB, wavesB, waveLengthNetwork, "bLSP", id, protectionType, thresholdObj, vector<vector<int>>{ createRemaingBackUpDeatils.w1ShortPathBP, createRemaingBackUpDeatils.w2ShortPathBP});
                                    lspObj.makeLSP(bandwidth, pathForPLSP, wholePathP, wavesP, waveLengthNetwork, "pLSP", id, protectionType, thresholdObj);

                                    myfile.writeLog("New LSP establish with single LP for primary, and 2 combine LPs for backup. [" + to_string(wavesP[0]) + "] [" + to_string(wavesB[0]) + "," + to_string(wavesB[1]) + "]");

                                    lspPathDetails[id][0][0] = pathForPLSP;
                                    lspPathDetails[id][0][1] = wavesP;
                                    lspPathDetails[id][1][0] = pathForBLSP;
                                    lspPathDetails[id][1][1] = wavesB;

                                    isLSPestablish = true;
                                    oldNcombine++;

                                }
                            }
                        }
                    }
                    else pathNotFound++;
                    
                    if (isLSPestablish) AlreadyPathEstablish++;
                    else AlreadyPathReject++;
                }

                
                if(!isLSPestablish) 
                {

                    rejectedEvents.push_back(listOfEvents[0].identifier);
                    rejected++;
                    myfile.writeLog("LSP is REJECTED.");
                }
            }
            

            else if(!action)
            {

                 myfile.writeLog(("New release. Bandwidth = " + to_string(bandwidth) + ",source = " + to_string(source) + ", Dst = "
                    + to_string(destination) + ", id = " + to_string(id) + ", Release = " + to_string(action)));


                 vector<int> pathP = lspPathDetails[id][0][0];
                 vector<int> wavesP = lspPathDetails[id][0][1];
                 vector<int> pathB = lspPathDetails[id][1][0];
                 vector<int> wavesB = lspPathDetails[id][1][1];

                if (pathP[0] == -1 || wavesP[0] == -1 || pathB[0] == -1 || wavesB[0] == -1)
                {

                }
                else
                {
                    myfile.writeLog(("New release. Bandwidth = " + to_string(bandwidth) + ",source = " + to_string(source) + ", Dst = "
                        + to_string(destination) + ", id = " + to_string(id) + ", Release = " + to_string(action)));

                    //lspObj.releaseLSP(pathB, wavesB, waveLengthNetwork, id, thresholdObj, protectionType);
                    lspObj.releaseLSP(pathP, wavesP, waveLengthNetwork, id, thresholdObj, protectionType, "pLSP", _couter);
                    lspObj.releaseLSP(pathB, wavesB, waveLengthNetwork, id, thresholdObj, protectionType, "bLSP", _couter);

               
                }

            }

            if (!listOfEvents.empty())
            {
                itr = listOfEvents.begin();
                listOfEvents.erase(itr);
            }
            
        }

        cout << "\n**Remain LPs_\n";
        waveLengthNetwork.viewAllLighpaths();
        cout << "\n\n**Remain LSPs_\n";
        lspObj.viewLSPsInALightpath(waveLengthNetwork);

        //waveLengthNetwork.viewAllLighpaths();
        myfile.writeLog("");
        myfile.writeLog("*****************");
        if(protectionType)
        {
            myfile.writeLog("Protection type = Bandwidth based");
            myfile.writeLog("Threshold value = "+to_string(thresholdObj.bandwidthThreshold));
        }
        else
        {
            myfile.writeLog("Protection type = Number of LSP based");
            myfile.writeLog("Threshold value = "+to_string(thresholdObj.numLSPthreshold));
        }
        myfile.writeLog("Erlang = "+to_string(erlang));
        myfile.writeLog("Mean holding time = "+to_string(meanHoldingTime));
        myfile.writeLog("Capacity of a LP = 10");
        myfile.writeLog("Mean holding time = " + to_string(meanHoldingTime));
        myfile.writeLog("Num.of wave lengths per fiber = "+to_string(numOfWaves));

        myfile.writeLog("                 ");
        myfile.writeLog("****Counts****");
        myfile.writeLog("Num.of lsp rqst = "+to_string(numberOfLSPrequests));
        myfile.writeLog(" ");
        myfile.writeLog("*Num.of established LSPs = " + to_string(noAlreadyPathEstablish + AlreadyPathEstablish));
        myfile.writeLog("*Num.of rejected LSPs = " + to_string(rejected));
        
        myfile.writeLog(" ");
        myfile.writeLog("**From no already path_");
        myfile.writeLog("newLPnNewLP = " + to_string(newLPnNewLP));
        myfile.writeLog("newLPnCombineLP = " + to_string(newLPnCombineLP));
        myfile.writeLog("combineLPnCombineLP = " + to_string(combineLPnCombineLP));
        myfile.writeLog("Established = " + to_string(noAlreadyPathEstablish));
        myfile.writeLog("Rejected = " + to_string(noAlreadyPathReject));
        
        myfile.writeLog(" ");
        myfile.writeLog("**From already path_");
        myfile.writeLog("tempNAlready = " + to_string(tempNAlready));
        myfile.writeLog("tempNtemp = " + to_string(tempNtemp));
        myfile.writeLog("tempNcombine = " + to_string(tempNcombine));
        myfile.writeLog("oldNold = " + to_string(oldNold));
        myfile.writeLog("oldNnew = " + to_string(oldNnew));
        myfile.writeLog("oldNcombine = " + to_string(oldNcombine));
        myfile.writeLog("Established = " + to_string(AlreadyPathEstablish));
        myfile.writeLog("Rejected = " + to_string(AlreadyPathReject));
        myfile.writeLog(" ");
        myfile.writeLog("pathNotFound = " + to_string(pathNotFound));
        
    }

    //path can create
    int newLPnNewLP = 0;
    int newLPnCombineLP = 0;
    int combineLPnCombineLP = 0;

   
    cout << "\nPress ENTER to exit\n";
	cin.get();
    return 0;
}
