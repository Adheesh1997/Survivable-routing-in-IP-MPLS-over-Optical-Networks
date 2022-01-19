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
    bool hybrid = false;
    thresholdObj.bandwidthThreshold = 0.5;  //Assigning the threshold values
    thresholdObj.numLSPthreshold = 1;        //Assigning the threshold values
    int numberOfLSPrequests = 5000;           //The number of LSP requests
    double erlang = 70;                      //Erlang value
    double meanHoldingTime = 1;              //Mean holding time
    int numOfWaves = 16;
    int bandwidthCap = 100;
    thresholdObj.bandwidthCap = bandwidthCap;
    thresholdObj.protectionType = protectionType;
    thresholdObj.hybrid = hybrid;

    requestCreation tempObject;
    tempObject.requestGenerator(numberOfLSPrequests, erlang, meanHoldingTime, bandwidthCap);   //Create the LSP requests
    //tempObject.printLSPrequests();                                             //Print the LSP requests
    //
    //tempObject.printEvents();                                                  //Print the events
    vector<int> rejectedEvents;                                                  //To capture the rejected events
    vector<events>::iterator itr;

    // Only (1) or (2) keep uncomment at one time , Dont both or Dont keep both comment!!!!!

    /*************** Read event to a file*****************  -------------------------(1)  **/
    vector<events> listOfEvents = tempObject.eventCreation();                    //Create the events
    myfile.wrteALSP("rqst_inputs/rq_test_2022_01_19.txt", listOfEvents); 
    /*************** end of (1) *******************/

    /**************** LSP requests read from file ********** -------------------------(2) */
    //vector<events> listOfEvents;
    //myfile.readLSPs("rqst_inputs/rq_test_2022_01_17-3.txt", listOfEvents);
    /*************** end of (2) *******************/

    vector<vector<int>> adjacencyMetrix; //Vector to store adjacency metrix that represent netork

    map<int, map<int,vector<vector<int>>>> lspPathDetails;

    //graph input file location
    string fileLocation = "graph_inputs/05/graph05.csv"; 
    
    int theCount = 1;
    //Read csv file and assign values to the matrix 
    if(myfile.readGraphInputFile(numOfNodes, adjacencyMetrix,fileLocation,numOfWaves))
    { 
        //myfile.writeLog((fileLocation + " Graph is imported."));
        cout << "Graph imported from csv file.\n";
        //If there is no any error while reading file then graph is created
        fiberLinkNetwork physicalLinkNetwork(numOfNodes, numOfWaves);
        thresholdObj.numOfNodesOfTheNetwork = numOfNodes;

        physicalLinkNetwork.setupFiberLinkNetwork(adjacencyMetrix,numOfWaves);
        cout << "Physical link network is created.\n";

        //myfile.writeLog("Physical network is created.");
        //physicalLinkNetwork.printGraph();

        //Generate sub wavelength graphs for establish LPs
        vector<waveLengthNetworks> subWaveNetworks = setupWaveLengthNetworks(adjacencyMetrix, numOfWaves);
        cout << "subWaveNetworks = " << subWaveNetworks.size() << endl;

        waveLengthNetworks defaulSubWaveNetworks = subWaveNetworks[0];
        
        lightpathNetwork waveLengthNetwork(subWaveNetworks);
        
        
        cout << endl << "Generting all LSP requests.\n";
        lspRequestGenarator lspReqGen(numOfNodes);
        lspRequest lspReq;

        LSP lspObj(&subWaveNetworks);


        //New counters
        int rejected = 0;
        int pathNotFound = 0;
        int BPfalsePPtrue = 0;
        int BPfalsePPfalse = 0;

        int noAlreadyPathEstablish = 0;
        int noAlreadyPathReject = 0;
            int newLPnNewLP = 0;
            int newLPnCombineLP = 0;
            int combineLPnCombineLP = 0;
            int newMethod = 0;


        int AlreadyPathEstablish = 0;
        int AlreadyPathReject = 0;
            int tempNAlready = 0;
            int tempNtemp = 0;
            int tempNcombine = 0;
            int oldNold = 0;
            int oldNnew = 0;
            int oldNcombine = 0;

        //int eventCounter = 0, actiontrueCounter = 0, establishedLSPcounter = 0, rejectedLSPcounter = 0, releasedLSPcounter = 0;

            vector<int> bandwidthEstablishCount(11, 0);
            vector<int> bandwidthRejectCount(11, 0);
        
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
                //actiontrueCounter++;
                cout << "\nRqst : " << id << endl;
                
                map<int, map<int, vector<vector<int>>>> mapFromLPGraph = waveLengthNetwork.mapFromLpGraph(numOfWaves, numOfNodes, bandwidth);

                moreOEOConvertion moreCovertions = createMainGraph(vexnum, subWaveNetworks, source, destination, mapFromLPGraph);

                if (moreCovertions.canCreateBP && moreCovertions.canCreatePP)
                {
                    //establishedLSPcounter++;
                    // Creating LPs for primary and backup paths
                    for (int primaryPart = 0; primaryPart < moreCovertions.wavelengthNumberPP.size(); primaryPart++)
                    {
                        if (moreCovertions.wavelengthNumberPP[primaryPart] < numOfWaves)
                        {
                            checkWhetherLPidfinished(LPids, countForLPids);
                            waveLengthNetwork.setANewLighpath(moreCovertions.allPathDetailsPP[primaryPart], moreCovertions.wavelengthNumberPP[primaryPart], "pp", LPids[0], bandwidthCap);
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
                            waveLengthNetwork.setANewLighpath(moreCovertions.allPathDetailsBP[backupPart], moreCovertions.wavelengthNumberBP[backupPart], "pp", LPids[0], bandwidthCap);
                            removeLink(moreCovertions.wavelengthNumberBP[backupPart], moreCovertions.allPathDetailsBP[backupPart], subWaveNetworks);
                            removeLPidFromVec(LPids);
                        }
                        else
                        {
                            moreCovertions.wavelengthNumberBP[backupPart] -= numOfWaves;
                        }
                    }

                    // Creating LSPs for primary and backup
                    vector<int> wholePathP = { moreCovertions.allPathDetailsPP[0][0] };
                    vector<int> pathForPLSP = { moreCovertions.allPathDetailsPP[0][0] };
                    vector<int> wavesP = moreCovertions.wavelengthNumberPP;

                    for (int primaryPart = 0; primaryPart < moreCovertions.allPathDetailsPP.size(); primaryPart++)
                    {
                        wholePathP.insert(wholePathP.end(), moreCovertions.allPathDetailsPP[primaryPart].begin() + 1, moreCovertions.allPathDetailsPP[primaryPart].end());
                        pathForPLSP.push_back(moreCovertions.allPathDetailsPP[primaryPart].back());

                    }

                    vector<int> wholePathB = { moreCovertions.allPathDetailsBP[0][0] };
                    vector<int> pathForBLSP = { moreCovertions.allPathDetailsBP[0][0] };
                    vector<int> wavesB = moreCovertions.wavelengthNumberBP;

                    for (int backupPart = 0; backupPart < moreCovertions.allPathDetailsBP.size(); backupPart++)
                    {
                        wholePathB.insert(wholePathB.end(), moreCovertions.allPathDetailsBP[backupPart].begin() + 1, moreCovertions.allPathDetailsBP[backupPart].end());
                        pathForBLSP.push_back(moreCovertions.allPathDetailsBP[backupPart].back());

                    }

                    
                    lspObj.makeLSP(bandwidth, pathForPLSP, wholePathP, wavesP, waveLengthNetwork, "pLSP", id, protectionType, thresholdObj, hybrid, moreCovertions.allPathDetailsPP);
                    lspObj.makeLSP(bandwidth, pathForBLSP, wholePathB, wavesB, waveLengthNetwork, "bLSP", id, protectionType, thresholdObj, hybrid, moreCovertions.allPathDetailsBP);

                    isLSPestablish = true;
                    combineLPnCombineLP++;

                    lspPathDetails[id][0][0] = pathForPLSP;
                    lspPathDetails[id][0][1] = wavesP;
                    lspPathDetails[id][1][0] = pathForBLSP;
                    lspPathDetails[id][1][1] = wavesB;


                }

                if (!moreCovertions.canCreateBP && moreCovertions.canCreatePP)
                {
                    BPfalsePPtrue++;
                }

                if (!moreCovertions.canCreateBP && !moreCovertions.canCreatePP)
                {
                    BPfalsePPfalse++;
                }

                if (isLSPestablish) noAlreadyPathEstablish++;
                else
                {
                    noAlreadyPathReject++;
                    //rejectedLSPcounter++;
                }
            }
            

            else if(!action)
            {

                 //myfile.writeLog(("New release. Bandwidth = " + to_string(bandwidth) + ",source = " + to_string(source) + ", Dst = "
                   // + to_string(destination) + ", id = " + to_string(id) + ", Release = " + to_string(action)));

                 //releasedLSPcounter++;

                 vector<int> pathP = lspPathDetails[id][0][0];
                 vector<int> wavesP = lspPathDetails[id][0][1];
                 vector<int> pathB = lspPathDetails[id][1][0];
                 vector<int> wavesB = lspPathDetails[id][1][1];

                if (pathP[0] == -1 || wavesP[0] == -1 || pathB[0] == -1 || wavesB[0] == -1)
                {

                }
                else
                {
                   // myfile.writeLog(("New release. Bandwidth = " + to_string(bandwidth) + ",source = " + to_string(source) + ", Dst = "
                     //   + to_string(destination) + ", id = " + to_string(id) + ", Release = " + to_string(action)));

                    //lspObj.releaseLSP(pathB, wavesB, waveLengthNetwork, id, thresholdObj, protectionType);

                    //lspObj.releaseLSP(pathP, wavesP, waveLengthNetwork, id, thresholdObj, protectionType, "pLSP", _couter, hybrid);
                    lspObj.newReleaseLSP(pathP, wavesP, waveLengthNetwork, id, thresholdObj, protectionType, "pLSP", hybrid);
                    //lspObj.releaseLSP(pathB, wavesB, waveLengthNetwork, id, thresholdObj, protectionType, "bLSP", _couter, hybrid);
                    lspObj.newReleaseLSP(pathB, wavesB, waveLengthNetwork, id, thresholdObj, protectionType, "bLSP", hybrid);
                }

            }

            if (!listOfEvents.empty())
            {
                itr = listOfEvents.begin();
                listOfEvents.erase(itr);
            }
            
        }

        vector<float> finalValues = waveLengthNetwork.averageCalculation();

        vector<float> HLLPvalues = waveLengthNetwork.heavilyLPAndOtherAvgCalc();

        cout << "\n**Remain LPs_\n";
        waveLengthNetwork.viewAllLighpaths();
        cout << "\n\n**Remain LSPs_\n";
        lspObj.viewLSPsInALightpath(waveLengthNetwork);

        cout << "\nEstablish count_";
        for (int qq = 0; qq < bandwidthEstablishCount.size(); qq++)
        {
            cout << qq << "\t: " << bandwidthEstablishCount[qq] << endl;
        }
        cout << "\nReject count_";
        for (int qq = 0; qq < bandwidthRejectCount.size(); qq++)
        {
            cout << qq << "\t: " << bandwidthRejectCount[qq] << endl;
        }

        myfile.createLog();
        //waveLengthNetwork.viewAllLighpaths();
        myfile.writeLog("");
        myfile.writeLog("*****************");
        if (hybrid)
        {
            myfile.writeLog("Protection type = Hybrid version");
            myfile.writeLog("Bandwidth threshold value = " + to_string(thresholdObj.bandwidthThreshold));
            myfile.writeLog("# of LSP threshold value = " + to_string(thresholdObj.numLSPthreshold));
        }
        else
        {
            if (protectionType)
            {
                myfile.writeLog("Protection type = Bandwidth based");
                myfile.writeLog("Threshold value = " + to_string(thresholdObj.bandwidthThreshold));
            }
            else
            {
                myfile.writeLog("Protection type = Number of LSP based");
                myfile.writeLog("Threshold value = " + to_string(thresholdObj.numLSPthreshold));
            }
        }
        
        myfile.writeLog("Graph = " + fileLocation); 
        myfile.writeLog("Number of nodes = " + to_string(numOfNodes));
        myfile.writeLog("Erlang = "+to_string(erlang));
        myfile.writeLog("Capacity of a LP = " + to_string(bandwidthCap));
        myfile.writeLog("Mean holding time = " + to_string(meanHoldingTime));
        myfile.writeLog("Num.of wave lengths per fiber = "+to_string(numOfWaves));

        myfile.writeLog("                 ");
        myfile.writeLog("****Counts****");
        myfile.writeLog("Num.of lsp rqst = " + to_string(numberOfLSPrequests));
        myfile.writeLog("Num.of BPfalse & PPtrue rqst = " + to_string(BPfalsePPtrue));
        myfile.writeLog("Num.of BPfalse & PPfalse rqst = " + to_string(BPfalsePPfalse));
        /*myfile.writeLog(" ");
        myfile.writeLog("*Num.of established LSPs = " + to_string(noAlreadyPathEstablish + AlreadyPathEstablish));
        myfile.writeLog("*Num.of rejected LSPs = " + to_string(rejected));
        
        myfile.writeLog(" ");
        myfile.writeLog("**From no already path_");
        myfile.writeLog("newLPnNewLP = " + to_string(newLPnNewLP));
        myfile.writeLog("newLPnCombineLP = " + to_string(newLPnCombineLP));
        myfile.writeLog("combineLPnCombineLP = " + to_string(combineLPnCombineLP));
        myfile.writeLog("newMethod = " + to_string(newMethod));*/
        myfile.writeLog("Established = " + to_string(noAlreadyPathEstablish));
        myfile.writeLog("Rejected = " + to_string(noAlreadyPathReject));
        float blockingProbability = static_cast<float>(noAlreadyPathReject) / static_cast<float>(numberOfLSPrequests);
        myfile.writeLog("Blocking Probability = " + to_string(blockingProbability));
        
        myfile.writeLog(" ");
        myfile.writeLog("**Calculated values_");
        myfile.writeLog("Average primary LSP bandwidth in backup LP= " + to_string(finalValues[0]));
        myfile.writeLog("Average primary LSP count in backup LP = " + to_string(finalValues[1]));
        myfile.writeLog("Possible bakup LP count= " + to_string(finalValues[2]));
        myfile.writeLog("Rejected backup LP count = " + to_string(finalValues[3]));
        myfile.writeLog("Established backup LP count = " + to_string(finalValues[4]));
        myfile.writeLog(" ");

        myfile.writeLog("Average Heavy Lightpath Protection Probability = " + to_string(HLLPvalues[0]));
        myfile.writeLog("Average primary LSP count of HLLP = " + to_string(HLLPvalues[1]));
        myfile.writeLog("Average primary LSP bandwidth of HLLP = " + to_string(HLLPvalues[2]));
        myfile.writeLog("Average primary LSP bandwidth of all LP = " + to_string(HLLPvalues[3]));
        myfile.writeLog("Percentage of Heavily Loaded Lightpath = " + to_string(HLLPvalues[4]));
        myfile.writeLog(" ");
        
    }


   
    cout << "\nPress ENTER to exit\n";
	cin.get();
    return 0;
}
