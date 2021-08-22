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

    thresholds thresholdObj;
    bool protectionType = false;              //True for bandwidth based LP protection. False for number of LSPs based LP protection
    thresholdObj.bandwidthThreshold = 0.2;  //Assigning the threshold values
    thresholdObj.numLSPthreshold = 1;        //Assigning the threshold values
    int numberOfLSPrequests = 5;           //The number of LSP requests
    double erlang = 130;                      //Erlang value
    double meanHoldingTime = 1;              //Mean holding time
    int numOfWaves = 40;

    requestCreation tempObject;
    tempObject.requestGenerator(numberOfLSPrequests, erlang, meanHoldingTime);   //Create the LSP requests
    //tempObject.printLSPrequests();                                             //Print the LSP requests
    //
    //tempObject.printEvents();                                                  //Print the events
    vector<int> rejectedEvents;                                                  //To capture the rejected events
    vector<events>::iterator itr;

    // Only (1) or (2) keep uncomment at one time , Dont both or Dont keep both comment!!!!!

    /*************** Read event to a file*****************  -------------------------(1)  **/
    /* vector<events> listOfEvents = tempObject.eventCreation();                    //Create the events
    myfile.wrteALSP("rqst_inputs/rq.txt", listOfEvents); */
    /*************** end of (1) *******************/

    /**************** LSP requests read from file ********** -------------------------(2) */
    vector<events> listOfEvents;
    myfile.readLSPs("rqst_inputs/rq.txt",listOfEvents);
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

        int newLPnoldLP = 0;
        int newLP = 0;
        int oldLP = 0;
        int rejected = 0;
        int totalCount = 0;
        
        vector<int> LPids(100000); // vector with 100000 ints.
        iota(begin(LPids), end(LPids), 1); // Fill with 1, 2, ..., 100000.
        int countForLPids = 100000;
        
        cout << "Establishing and releasing LSP requests.....\n";
        vector<int> pPathFortest;
        vector<int> bPathFortest;
        vector<int> waveVecFortest;

        cout<<"Before\n";
        print2dVector(subWaveNetworks[0].waveAdjacancyMatrix);

        while(!listOfEvents.empty())//for(events event:tempObject.eventVector)
        //for(int pp = 0; pp < listOfEvents.size(); pp++)
        {
            
            int vexnum = numOfNodes;
            int source = listOfEvents[0].sourceNode;
            int destination = listOfEvents[0].destinationNode;
            int id = listOfEvents[0].identifier;
            int bandwidth = 10; //listOfEvents[0].bandwidth;
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
                totalCount++;
                myfile.writeLog(("New request. Bandwidth = "+to_string(bandwidth)+",source = "+to_string(source)+", Dst = "
                                +to_string(destination)+", id = "+to_string(id)+", request = "+to_string(action)));

                vector<vector<int>> adjMetrixForPrimaryLSP = waveLengthNetwork.lpPAdjacencyMetrix(bandwidth, numOfNodes);
                            
                findPathDetails pathDetails = startingPoint(vexnum, subWaveNetworks, source, destination,adjMetrixForPrimaryLSP);

                map<int, map<int, vector<vector<int>>>> mapFromLPGraph;

                //If there is no LP(S) to establish LSP
                if (pathDetails.alreadyPPhave == false) 
                {

                    if(pathDetails.canCreatBP && pathDetails.canCreatPP && !isLSPestablish)
                    {
                        checkWhetherLPidfinished(LPids, countForLPids);
                        waveLengthNetwork.setANewLighpath(pathDetails.primaryShortPath, pathDetails.wavelengthNoPP,"pp", LPids[0]);
                        removeLPidFromVec(LPids);

                        checkWhetherLPidfinished(LPids, countForLPids);
                        waveLengthNetwork.setANewLighpath(pathDetails.backUpShortPath,pathDetails.wavelengthNoBP,"pp", LPids[0]);
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

                        myfile.writeLog("New lsp established with new single LP. ["+to_string(pathDetails.wavelengthNoPP)+"] ["+to_string(pathDetails.wavelengthNoBP)+"]");
                        isLSPestablish = true;
                        newLP++;
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
                                removeLPidFromVec(LPids);

                            }
                            else
                            {
                                createRemaingBackUpDeatils.wavelengthNo2BP -= numOfWaves;
                            }
                            checkWhetherLPidfinished(LPids, countForLPids);
                            waveLengthNetwork.setANewLighpath(pathDetails.primaryShortPath, pathDetails.wavelengthNoPP,"pp", LPids[0]);
                            removeLPidFromVec(LPids);

                            vector<int>wholePathP = pathDetails.primaryShortPath;
                            vector<int> pathForPLSP = {pathDetails.primaryShortPath[0],pathDetails.primaryShortPath[pathDetails.primaryShortPath.size()-1]};
                            vector<int> wavesP = {pathDetails.wavelengthNoPP};
                            
                            vector<int> wholePathB = createRemaingBackUpDeatils.w1ShortPathBP;
                            wholePathB.insert(wholePathB.end(), createRemaingBackUpDeatils.w2ShortPathBP.begin() + 1, createRemaingBackUpDeatils.w2ShortPathBP.end());

                            vector<int> pathForBLSP = {source, createRemaingBackUpDeatils.connectingNodeBP, destination};
                            vector<int> wavesB = {createRemaingBackUpDeatils.wavelengthNo1BP, createRemaingBackUpDeatils.wavelengthNo2BP};

                            lspObj.makeLSP(bandwidth, pathForBLSP, wholePathB, wavesB, waveLengthNetwork, "bLSP", id, protectionType, thresholdObj);
                            lspObj.makeLSP(bandwidth, pathForPLSP, wholePathP, wavesP, waveLengthNetwork, "pLSP", id, protectionType, thresholdObj);

                            myfile.writeLog("New LSP establish with single LP for primary, and 2 combine LPs for backup. ["+to_string(wavesP[0])+"] ["+to_string(wavesB[0])+","+to_string(wavesB[1])+"]");
                            
                            lspPathDetails[id][0][0] = pathForPLSP;
                            lspPathDetails[id][0][1] = wavesP;
                            lspPathDetails[id][1][0] = pathForBLSP;
                            lspPathDetails[id][1][1] = wavesB;

                            isLSPestablish = true;
                            newLP++;

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
                                removeLPidFromVec(LPids);

                            }
                            
                            else combineWavelengthDetails.wavelengthNo1PP -= numOfWaves;

                            if (combineWavelengthDetails.wavelengthNo2PP < numOfWaves)
                            {
                                checkWhetherLPidfinished(LPids, countForLPids);
                                waveLengthNetwork.setANewLighpath(combineWavelengthDetails.w2ShortPathPP, combineWavelengthDetails.wavelengthNo2PP, "pp", LPids[0]);
                                removeLPidFromVec(LPids);

                            }

                            else combineWavelengthDetails.wavelengthNo2PP -= numOfWaves;


                            if (combineWavelengthDetails.wavelengthNo1BP < numOfWaves)
                            {
                                checkWhetherLPidfinished(LPids, countForLPids);
                                waveLengthNetwork.setANewLighpath(combineWavelengthDetails.w1ShortPathBP, combineWavelengthDetails.wavelengthNo1BP, "pp", LPids[0]);
                                removeLPidFromVec(LPids);

                            }
                            
                            else combineWavelengthDetails.wavelengthNo1BP -= numOfWaves;

                            if (combineWavelengthDetails.wavelengthNo2BP < numOfWaves)
                            {
                                checkWhetherLPidfinished(LPids, countForLPids);
                                waveLengthNetwork.setANewLighpath(combineWavelengthDetails.w2ShortPathBP, combineWavelengthDetails.wavelengthNo2BP, "pp", LPids[0]);
                                removeLPidFromVec(LPids);

                            }
                            
                            else combineWavelengthDetails.wavelengthNo2BP -= numOfWaves;

                            vector<int> wholePathP = combineWavelengthDetails.w1ShortPathPP;
                            wholePathP.insert(wholePathP.end(), combineWavelengthDetails.w2ShortPathPP.begin() + 1, combineWavelengthDetails.w2ShortPathPP.end());
                            vector<int> pathForPLSP = {source, combineWavelengthDetails.connectingNodePP, destination};
                            vector<int> wavesP = {combineWavelengthDetails.wavelengthNo1PP, combineWavelengthDetails.wavelengthNo2PP};

                            lspObj.makeLSP(bandwidth, pathForPLSP, wholePathP, wavesP, waveLengthNetwork, "pLSP", id, protectionType, thresholdObj);

                            vector<int> wholePathB = combineWavelengthDetails.w1ShortPathBP;
                            wholePathB.insert(wholePathB.end(), combineWavelengthDetails.w2ShortPathBP.begin() + 1, combineWavelengthDetails.w2ShortPathBP.end());
                            vector<int> pathForBLSP = {source, combineWavelengthDetails.connectingNodeBP, destination};
                            vector<int> wavesB = {combineWavelengthDetails.wavelengthNo1BP, combineWavelengthDetails.wavelengthNo2BP};

                            lspObj.makeLSP(bandwidth, pathForBLSP, wholePathB, wavesB, waveLengthNetwork, "bLSP", id, protectionType, thresholdObj);

                            myfile.writeLog("New LSP establish with combine LPs for both primary and backup LSPs. ["
                                            +to_string(wavesP[0])+","+to_string(wavesP[1])+"] ["+to_string(wavesB[0])+","+to_string(wavesB[1])+"]");
                            
                            lspPathDetails[id][0][0] = pathForPLSP;
                            lspPathDetails[id][0][1] = wavesP;
                            lspPathDetails[id][1][0] = pathForBLSP;
                            lspPathDetails[id][1][1] = wavesB;

                            isLSPestablish = true;
                            newLP++;
                        }
                    }

                    
                    
                }

                else
                {  
                    if(pathDetails.tempCanCreatPP)
                    {
                        vector<vector<int>> waveVector = waveLengthNetwork.getWaveNumbers(source,destination,numOfNodes, bandwidth, pathDetails.tempPrimaryShortPath.size());

                        if(waveVector[0][0] == -1)
                        {
                            vector<int> backupWave = waveLengthNetwork.getBWaveNumber(source,destination,numOfNodes,bandwidth,pathDetails.tempPrimaryShortPath);

                            if(backupWave[0] != -1)
                            {
                                checkWhetherLPidfinished(LPids, countForLPids);
                                waveLengthNetwork.setANewLighpath(pathDetails.tempPrimaryShortPath,pathDetails.tempWavelengthNoPP,"pp", LPids[0]);
                                removeLPidFromVec(LPids);

                                subWaveNetworks[pathDetails.tempWavelengthNoPP].removeLink(source,destination);
                                
                                vector<int> wholePathP = {pathDetails.tempPrimaryShortPath};
                                vector<int> wholePathB;
                                wholePathB.assign(backupWave.begin()+1, backupWave.end());

                                vector<int> pathForLSP = {source,destination};
                                vector<int> wavesP = {pathDetails.tempWavelengthNoPP};
                                vector<int> wavesB = {backupWave[0]};

                                lspObj.makeLSP(bandwidth, pathForLSP, wholePathP, wavesP, waveLengthNetwork, "pLSP", id, protectionType, thresholdObj);
                                lspObj.makeLSP(bandwidth, pathForLSP, wholePathB, wavesB, waveLengthNetwork, "bLSP", id, protectionType, thresholdObj);
                                
                                lspPathDetails[id][0][0] = pathForLSP;
                                lspPathDetails[id][0][1] = wavesP;
                                lspPathDetails[id][1][0] = pathForLSP;
                                lspPathDetails[id][1][1] = wavesB;

                                myfile.writeLog("New lsp establish with new LP and old LP. [" +to_string(wavesP[0])+"] ["+to_string(wavesB[0])+"]");
                                newLPnoldLP++;
                                isLSPestablish = true;
                            }
                        }
                        else
                        {
                            vector<int> wholePathP;
                            wholePathP.assign(waveVector[0].begin()+1, waveVector[0].end());

                            vector<int> wholePathB;
                            wholePathB.assign(waveVector[1].begin()+1, waveVector[1].end());

                            vector<int> pathForLSP = {source,destination};
                            vector<int> wavesP = {waveVector[0][0]};
                            vector<int> wavesB = {waveVector[1][0]};

                            lspObj.makeLSP(bandwidth, pathForLSP, wholePathP, wavesP, waveLengthNetwork,"pLSP",id, protectionType, thresholdObj);
                            lspObj.makeLSP(bandwidth, pathForLSP, wholePathB, wavesB, waveLengthNetwork,"bLSP",id, protectionType, thresholdObj);
                            
                            lspPathDetails[id][0][0] = pathForLSP;
                            lspPathDetails[id][0][1] = wavesP;
                            lspPathDetails[id][1][0] = pathForLSP;
                            lspPathDetails[id][1][1] = wavesB;

                            myfile.writeLog("New lsp established with 2 old LPs. [" +to_string(wavesP[0])+"] ["+to_string(wavesB[0])+"]");
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
            

            else if(!action)
            {

                 vector<int> pathP = lspPathDetails[id][0][0];
                 vector<int> wavesP = lspPathDetails[id][0][1];
                 vector<int> pathB = lspPathDetails[id][1][0];
                 vector<int> wavesB = lspPathDetails[id][1][1];

                if (pathP[0] == -1 || wavesP[0] == -1 || pathB[0] == -1 || wavesB[0] == -1)
                {
                    ///cout << "\nRejected lsp trying to release.";
                }
                else
                {
                    myfile.writeLog(("New release. Bandwidth = " + to_string(bandwidth) + ",source = " + to_string(source) + ", Dst = "
                        + to_string(destination) + ", id = " + to_string(id) + ", Release = " + to_string(action)));

                    lspObj.releaseLSP(pathP, wavesP, waveLengthNetwork, id, thresholdObj, protectionType);
                    lspObj.releaseLSP(pathB, wavesB, waveLengthNetwork, id, thresholdObj, protectionType);
                }

            }

            if (!listOfEvents.empty())
            {
                itr = listOfEvents.begin();
                listOfEvents.erase(itr);
            }
            
        } 

        cout<<"After\n";
        print2dVector(subWaveNetworks[0].waveAdjacancyMatrix);

        
           
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
        myfile.writeLog("Num.of wave lengths per fiber = "+to_string(numOfWaves));

        myfile.writeLog("                 ");
        myfile.writeLog("****Counts****");
        myfile.writeLog("Num.of lsp rqst = "+to_string(numberOfLSPrequests));
        myfile.writeLog("New LP & old LP = "+to_string(newLPnoldLP));
        myfile.writeLog("New LP = "+to_string(newLP));
        myfile.writeLog("old LP = "+to_string(oldLP));
        myfile.writeLog("                 ");
        myfile.writeLog("**Num.of LSP established = "+to_string(newLPnoldLP+newLP+oldLP));
        myfile.writeLog("**Num.of LSP rejected = "+to_string(rejected));
        myfile.writeLog("**Total count = "+to_string(totalCount));

    }

  
    cout << "\nPress ENTER to exit\n";
	cin.get();
    return 0;
}
