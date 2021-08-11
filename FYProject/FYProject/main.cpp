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


void removeLink(int waveNum, vector<int> path, vector<waveLengthNetworks>& subWaveNetworks)
{
    //cout<<"\nWave : "<<waveNum<<"\nPaht ";
    //printVector(path);
    for(int i = 0; i < path.size()-1; i++)
    {
        //cout<<"\nIn main"<<", Source : "<<path[i]<<" , Dst : "<<path[i+1]<<endl;
        subWaveNetworks[waveNum].removeLinks(path[i],path[i+1]);
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
    files myfile;
    int numOfNodes; //Variable to store number of nodes in the network

    thresholds thresholdObj;
    bool protectionType = false;              //True for bandwidth based LP protection. False for number of LSPs based LP protection
    thresholdObj.bandwidthThreshold = 0.2;  //Assigning the threshold values
    thresholdObj.numLSPthreshold = 1;        //Assigning the threshold values
    int numberOfLSPrequests = 5000;           //The number of LSP requests
    double erlang = 10;                      //Erlang value
    double meanHoldingTime = 1;              //Mean holding time


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
    myfile.wrteALSP("rqst_inputs\rq2.txt", listOfEvents); 
    /*************** end of (1) *******************/

    /**************** LSP requests read from file ********** -------------------------(2) */
    /*vector<events> listOfEvents;
    myfile.readLSPs("rqst_inputs/rq2.txt",listOfEvents);*/
    /*************** end of (2) *******************/

    vector<vector<int>> adjacencyMetrix; //Vector to store adjacency metrix that represent netork

    //graph input file location
    string fileLocation = "graph_inputs\05\graph05.csv"; 
    
    int theCount = 1;
    //Read csv file and assign values to the matrix 
    if(myfile.readGraphInputFile(numOfNodes, adjacencyMetrix,fileLocation))
    { 
        myfile.writeLog((fileLocation + " Graph is imported."));
        //If there is no any error while reading file then graph is created
        fiberLinkNetwork physicalLinkNetwork(numOfNodes, 40);
        thresholdObj.numOfNodesOfTheNetwork = numOfNodes;

        physicalLinkNetwork.setupFiberLinkNetwork(adjacencyMetrix);

        myfile.writeLog("Physical network is created.");
        //physicalLinkNetwork.printGraph();

        vector<waveLengthNetworks> subWaveNetworks = setupWaveLengthNetworks(adjacencyMetrix, 40);

        waveLengthNetworks defaulSubWaveNetworks = subWaveNetworks[0];
        
        lightpathNetwork waveLengthNetwork(subWaveNetworks);

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
        
        while(!listOfEvents.empty())//for(events event:tempObject.eventVector)
        {
            
            int vexnum = numOfNodes;
            int source = listOfEvents[0].sourceNode;
            int destination = listOfEvents[0].destinationNode;
            int id = listOfEvents[0].identifier;
            int bandwidth = 10; //listOfEvents[0].bandwidth;
            bool action = listOfEvents[0].action;

            //Generte a lsp reqest with src,dst,bandwidth, request or remove
            bool isLSPestablish = false;            
            for (int ww = 0; ww < rejectedEvents.size(); ww++)
            {
                if (listOfEvents[0].identifier == rejectedEvents[ww])
                {
                    itr = listOfEvents.begin();
                    listOfEvents.erase(itr);

                    vector<int>::iterator itr2;
                    itr2 = rejectedEvents.begin() + ww;
                    rejectedEvents.erase(itr2);
                }
            }

            if(action)
            {
                totalCount++;
                myfile.writeLog(("New request. Bandwidth = "+to_string(bandwidth)+",source = "+to_string(source)+", Dst = "
                                +to_string(destination)+", id = "+to_string(id)+", request = "+to_string(action)));

                
                //cout<<"\nLine: 156";
                vector<vector<int>> adjMetrixForPrimaryLSP = waveLengthNetwork.lpPAdjacencyMetrix(bandwidth, numOfNodes);
                //cout<<"\nLine: 158";
                             


                //cout<<"\nLine: 160";
                findPathDetails pathDetails = startingPoint(vexnum, subWaveNetworks, source, destination,adjMetrixForPrimaryLSP);

                map<int, map<int, vector<vector<int>>>> mapFromLPGraph;

                //If there is no LP(S) to establish LSP
                if (pathDetails.alreadyPPhave == false) 
                {
                    //If using new LPs can establish path for LSP reqeust
                    if(pathDetails.canCreatBP && pathDetails.canCreatPP && !isLSPestablish)
                    {
                        //cout<<"\nleni\n";
                        //printVector(pathDetails.primaryShortPath);
                        //cout<<"\nLine: 172";
                        checkWhetherLPidfinished(LPids, countForLPids);
                        waveLengthNetwork.setANewLighpath(pathDetails.primaryShortPath, pathDetails.wavelengthNoPP,"pp", LPids[0]);
                        removeLPidFromVec(LPids);

                        removeLink(pathDetails.wavelengthNoPP, pathDetails.primaryShortPath, subWaveNetworks);

                        checkWhetherLPidfinished(LPids, countForLPids);
                        waveLengthNetwork.setANewLighpath(pathDetails.backUpShortPath,pathDetails.wavelengthNoBP,"pp", LPids[0]);
                        removeLPidFromVec(LPids);

                        removeLink(pathDetails.wavelengthNoBP, pathDetails.backUpShortPath, subWaveNetworks);

                        vector<int> wholePathP = pathDetails.primaryShortPath;
                        vector<int> pathForPLSP = {pathDetails.primaryShortPath[0],pathDetails.primaryShortPath[pathDetails.primaryShortPath.size()-1]};
                        vector<int> wavesP = {pathDetails.wavelengthNoPP};
                        lspObj.makeLSP(pathForPLSP, wholePathP, wavesP, waveLengthNetwork, "pLSP", id, protectionType, thresholdObj);

                        vector<int> wholePathB = pathDetails.backUpShortPath;
                        vector<int> pathForBLSP = {pathDetails.backUpShortPath[0],pathDetails.backUpShortPath[pathDetails.backUpShortPath.size()-1]};
                        vector<int> wavesB = {pathDetails.wavelengthNoBP};
                        lspObj.makeLSP(pathForBLSP, wholePathB, wavesB, waveLengthNetwork, "bLSP", id, protectionType, thresholdObj);

                        myfile.writeLog("New lsp established with new single LP. ["+to_string(pathDetails.wavelengthNoPP)+"] ["+to_string(pathDetails.wavelengthNoBP)+"]");
                        isLSPestablish = true;
                        newLP++;
                    }
                    //cout<<"\nLine: 199";
                    map<int,vector<vector<int>>> arr = waveLengthNetwork.mapFromsource(source,numOfNodes, bandwidth);
                    map<int,vector<vector<int>>> arr1 = waveLengthNetwork.mapFromsource(destination,numOfNodes, bandwidth);
                    //cout<<"\nLine: 202";

                    //If LP can not creat for primary path
                    if(pathDetails.canCreatPP && !pathDetails.canCreatBP && !isLSPestablish)
                    {//cout<<"\nLine: 206";
                        forRemainingBackUpPath createRemaingBackUpDeatils = createRemaing(vexnum, subWaveNetworks, source, destination, arr, arr1, pathDetails.primaryShortPath);
                        //cout<<"\nLine: 208";
                        if(createRemaingBackUpDeatils.canCreatCombinationBP)
                        {
                            
                            if(createRemaingBackUpDeatils.wavelengthNo1BP < 40)
                            {
                                checkWhetherLPidfinished(LPids, countForLPids);
                                waveLengthNetwork.setANewLighpath(createRemaingBackUpDeatils.w1ShortPathBP,createRemaingBackUpDeatils.wavelengthNo1BP,"pp", LPids[0]);
                                removeLPidFromVec(LPids);

                                removeLink(createRemaingBackUpDeatils.wavelengthNo1BP, createRemaingBackUpDeatils.w1ShortPathBP, subWaveNetworks);
                            }
                            else
                            {
                                createRemaingBackUpDeatils.wavelengthNo1BP -= 40;
                            }

                            if(createRemaingBackUpDeatils.wavelengthNo2BP < 40)
                            {
                                checkWhetherLPidfinished(LPids, countForLPids);
                                waveLengthNetwork.setANewLighpath(createRemaingBackUpDeatils.w2ShortPathBP,createRemaingBackUpDeatils.wavelengthNo2BP,"pp", LPids[0]);
                                removeLPidFromVec(LPids);

                                removeLink(createRemaingBackUpDeatils.wavelengthNo2BP, createRemaingBackUpDeatils.w2ShortPathBP, subWaveNetworks);
                            }
                            else
                            {
                                createRemaingBackUpDeatils.wavelengthNo2BP -=40;
                            }
                            //cout<<"\nLine: 237";
                            checkWhetherLPidfinished(LPids, countForLPids);
                            waveLengthNetwork.setANewLighpath(pathDetails.primaryShortPath, pathDetails.wavelengthNoPP,"pp", LPids[0]);
                            removeLPidFromVec(LPids);
                            //cout<<"\nLine: 241";
                            removeLink(pathDetails.wavelengthNoPP, pathDetails.primaryShortPath, subWaveNetworks);

                            vector<int>wholePathP = pathDetails.primaryShortPath;
                            vector<int> pathForPLSP = {pathDetails.primaryShortPath[0],pathDetails.primaryShortPath[pathDetails.primaryShortPath.size()-1]};
                            vector<int> wavesP = {pathDetails.wavelengthNoPP};
                            lspObj.makeLSP(pathForPLSP, wholePathP, wavesP, waveLengthNetwork, "pLSP", id, protectionType, thresholdObj);
                            
                            vector<int> wholePathB = createRemaingBackUpDeatils.w1ShortPathBP;
                            wholePathB.insert(wholePathB.end(), createRemaingBackUpDeatils.w2ShortPathBP.begin() + 1, createRemaingBackUpDeatils.w2ShortPathBP.end());

                            vector<int> pathForBLSP = {source, createRemaingBackUpDeatils.connectingNodeBP, destination};
                            vector<int> wavesB = {createRemaingBackUpDeatils.wavelengthNo1BP, createRemaingBackUpDeatils.wavelengthNo2BP};

                            lspObj.makeLSP(pathForBLSP, wholePathB, wavesB, waveLengthNetwork, "bLSP", id, protectionType, thresholdObj);

                            myfile.writeLog("New LSP establish with single LP for primary, and 2 combine LPs for backup. ["+to_string(wavesP[0])+"] ["+to_string(wavesB[0])+","+to_string(wavesB[1])+"]");
                            
                            isLSPestablish = true;
                            newLP++;

                        }
                    }

                    if(!pathDetails.canCreatPP && !pathDetails.canCreatBP && !isLSPestablish)
                    {
                        //cout<<"\nLine: 266";
                        combineWavelength combineWavelengthDetails = pathCombinationCreat(vexnum, subWaveNetworks, source, destination, arr, arr1);
                        
                        if(combineWavelengthDetails.canCreatCombinationPP && combineWavelengthDetails.canCreatCombinationBP)
                        {//cout<<"\nLine: 270";
                            if (combineWavelengthDetails.wavelengthNo1PP < 40)
                            {
                                checkWhetherLPidfinished(LPids, countForLPids);
                                waveLengthNetwork.setANewLighpath(combineWavelengthDetails.w1ShortPathPP, combineWavelengthDetails.wavelengthNo1PP, "pp", LPids[0]);
                                removeLPidFromVec(LPids);

                                removeLink(combineWavelengthDetails.wavelengthNo1PP, combineWavelengthDetails.w1ShortPathPP, subWaveNetworks);
                            }
                            
                            else combineWavelengthDetails.wavelengthNo1PP -= 40;

                            if (combineWavelengthDetails.wavelengthNo2PP < 40)
                            {
                                checkWhetherLPidfinished(LPids, countForLPids);
                                waveLengthNetwork.setANewLighpath(combineWavelengthDetails.w2ShortPathPP, combineWavelengthDetails.wavelengthNo2PP, "pp", LPids[0]);
                                removeLPidFromVec(LPids);

                                removeLink(combineWavelengthDetails.wavelengthNo2PP, combineWavelengthDetails.w2ShortPathPP, subWaveNetworks);
                            }

                            else combineWavelengthDetails.wavelengthNo2PP -= 40;


                            if (combineWavelengthDetails.wavelengthNo1BP < 40)
                            {
                                checkWhetherLPidfinished(LPids, countForLPids);
                                waveLengthNetwork.setANewLighpath(combineWavelengthDetails.w1ShortPathBP, combineWavelengthDetails.wavelengthNo1BP, "pp", LPids[0]);
                                removeLPidFromVec(LPids);

                                removeLink(combineWavelengthDetails.wavelengthNo1BP, combineWavelengthDetails.w1ShortPathBP, subWaveNetworks);
                            }
                            
                            else combineWavelengthDetails.wavelengthNo1BP -= 40;

                            if (combineWavelengthDetails.wavelengthNo2BP < 40)
                            {
                                checkWhetherLPidfinished(LPids, countForLPids);
                                waveLengthNetwork.setANewLighpath(combineWavelengthDetails.w2ShortPathBP, combineWavelengthDetails.wavelengthNo2BP, "pp", LPids[0]);
                                removeLPidFromVec(LPids);

                                removeLink(combineWavelengthDetails.wavelengthNo2BP, combineWavelengthDetails.w2ShortPathBP, subWaveNetworks);
                            }
                            
                            else combineWavelengthDetails.wavelengthNo2BP -= 40;

                            vector<int> wholePathP = combineWavelengthDetails.w1ShortPathPP;
                            wholePathP.insert(wholePathP.end(), combineWavelengthDetails.w2ShortPathPP.begin() + 1, combineWavelengthDetails.w2ShortPathPP.end());
                            vector<int> pathForPLSP = {source, combineWavelengthDetails.connectingNodePP, destination};
                            vector<int> wavesP = {combineWavelengthDetails.wavelengthNo1PP, combineWavelengthDetails.wavelengthNo2PP};

                            lspObj.makeLSP(pathForPLSP, wholePathP, wavesP, waveLengthNetwork, "pLSP", id, protectionType, thresholdObj);

                            vector<int> wholePathB = combineWavelengthDetails.w1ShortPathBP;
                            wholePathB.insert(wholePathB.end(), combineWavelengthDetails.w2ShortPathBP.begin() + 1, combineWavelengthDetails.w2ShortPathBP.end());
                            vector<int> pathForBLSP = {source, combineWavelengthDetails.connectingNodeBP, destination};
                            vector<int> wavesB = {combineWavelengthDetails.wavelengthNo1BP, combineWavelengthDetails.wavelengthNo2BP};

                            lspObj.makeLSP(pathForBLSP, wholePathB, wavesB, waveLengthNetwork, "bLSP", id, protectionType, thresholdObj);

                            myfile.writeLog("New LSP establish with combine LPs for both primary and backup LSPs. ["
                                            +to_string(wavesP[0])+","+to_string(wavesP[1])+"] ["+to_string(wavesB[0])+","+to_string(wavesB[1])+"]");
                            
                            isLSPestablish = true;
                            newLP++;
                        }
                    }

                    
                    
                }

                //Already there is path(LP) for LSP request
                else
                {  
                    if(pathDetails.tempCanCreatPP)
                    {//cout<<"\nLine: 346";
                        vector<vector<int>> waveVector = waveLengthNetwork.getWaveNumbers(source,destination,numOfNodes, bandwidth, pathDetails.tempPrimaryShortPath.size());
                        //cout<<"\nLine: 348";
                        if(waveVector[0][0] == -1)
                        {//cout<<"\nLine: 350";
                            //find a backup path
                            vector<int> backupWave = waveLengthNetwork.getBWaveNumber(source,destination,numOfNodes,bandwidth,pathDetails.tempPrimaryShortPath);
                            //cout<<"\nLine: 353";
                            if(backupWave[0] != -1)
                            {
                                checkWhetherLPidfinished(LPids, countForLPids);
                                waveLengthNetwork.setANewLighpath(pathDetails.tempPrimaryShortPath,pathDetails.tempWavelengthNoPP,"pp", LPids[0]);
                                removeLPidFromVec(LPids);

                                removeLink(pathDetails.tempWavelengthNoPP, pathDetails.tempPrimaryShortPath, subWaveNetworks);

                                subWaveNetworks[pathDetails.tempWavelengthNoPP].removeLink(source,destination);
                                
                                vector<int> wholePathP = {pathDetails.tempPrimaryShortPath};
                                vector<int> wholePathB;
                                wholePathB.assign(backupWave.begin()+1, backupWave.end());

                                vector<int> pathForLSP = {source,destination};
                                vector<int> wavesP = {pathDetails.tempWavelengthNoPP};
                                vector<int> wavesB = {backupWave[0]};

                                lspObj.makeLSP(pathForLSP, wholePathP, wavesP, waveLengthNetwork, "pLSP", id, protectionType, thresholdObj);
                                lspObj.makeLSP(pathForLSP, wholePathB, wavesB, waveLengthNetwork, "bLSP", id, protectionType, thresholdObj);
                                
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

                            lspObj.makeLSP(pathForLSP, wholePathP, wavesP, waveLengthNetwork,"pLSP",id, protectionType, thresholdObj);
                            lspObj.makeLSP(pathForLSP, wholePathB, wavesB, waveLengthNetwork,"bLSP",id, protectionType, thresholdObj);
                            
                            
                            myfile.writeLog("New lsp established with 2 old LPs. [" +to_string(wavesP[0])+"] ["+to_string(wavesB[0])+"]");
                            oldLP++;
                            isLSPestablish = true;
                        }
                    }                
                }
                //cout<<endl<<"isEstablish = "<<isLSPestablish<<endl<<endl;
                if(!isLSPestablish) 
                {
                    rejectedEvents.push_back(listOfEvents[0].identifier);
                    rejected++;
                    myfile.writeLog("LSP is REJECTED.");
                }
                
                theCount++;
            }
            itr = listOfEvents.begin();
            listOfEvents.erase(itr);

            //cout << theCount << endl;
            
        } 
        
        //waveLengthNetwork.viewAllLighpaths();
        
        //print2dVector(subWaveNetworks[0].waveAdjacancyMatrix);
        //removeLink(0, vector<int>{1,3,4}, subWaveNetworks);
        //cout<<"\nAfter remove\n";
        //print2dVector(subWaveNetworks[0].waveAdjacancyMatrix);

        cout << theCount << endl;
        myfile.writeLog("                 ");
        myfile.writeLog("****Counts****");
        myfile.writeLog("Num of lsp rqst = "+to_string(numberOfLSPrequests));
        myfile.writeLog("new LP & old LP = "+to_string(newLPnoldLP));
        myfile.writeLog("new LP = "+to_string(newLP));
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