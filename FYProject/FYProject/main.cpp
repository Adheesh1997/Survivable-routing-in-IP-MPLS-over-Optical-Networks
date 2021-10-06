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

    thresholds thresholdObj;
    bool protectionType = true;              //True for bandwidth based LP protection. False for number of LSPs based LP protection
    int lightpathCapacity = 10;               //Lightpath bandwidth
    int lowestLSPcapacity = 1;                //Lowest bandwidth of a LSP
    int highestLSPcapacity = 10;              //Highest bandwidth of a LSP
    thresholdObj.lightpathCapacity = lightpathCapacity; //Assigning the lightpath bandwidth
    thresholdObj.bandwidthThreshold = 0.1;    //Assigning the threshold values
    thresholdObj.numLSPthreshold = 1;         //Assigning the threshold values
    int numberOfLSPrequests = 1000;           //The number of LSP requests
    double erlang = 10;                       //Erlang value
    double meanHoldingTime = 1;               //Mean holding time
    int numOfWaves = 16;                      //Number of wavelengths in a lightpath
    

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
        

        requestCreation tempObject;
        tempObject.requestGenerator(numberOfLSPrequests, erlang, meanHoldingTime, numOfNodes, lowestLSPcapacity, highestLSPcapacity);   //Create the LSP requests
        //tempObject.printLSPrequests();                                             //Print the LSP requests
        //                                                 //Print the events
        vector<int> rejectedEvents;                                                  //To capture the lspRejected events
        vector<events>::iterator itr;

        // Only (1) or (2) keep uncomment at one time , Dont both or Dont keep both comment!!!!!

        /*************** Read event to a file*****************  -------------------------(1)  **/
        vector<events> listOfEvents = tempObject.eventCreation();                    //Create the events
        //myfile.wrteALSP("rqst_inputs/rq10.txt", listOfEvents); 
        /*************** end of (1) *******************/

        //cout << "\n***New events***\n";
        //tempObject.printEvents(); 
        /**************** LSP requests read from file ********** -------------------------(2) */
        //vector<events> listOfEvents;
        //myfile.readLSPs("rqst_inputs/rq8.txt", listOfEvents);
        /*************** end of (2) *******************/


        //New counters
        int lspRejected = 0;
        int lspEstablished = 0;

        int lpRejected = 0;
        int lpEstablished = 0;

        //backupLpcount[0] = Number of backup LPs rejected;
        //backupLPcount[1] = Number of backup LPs established
        vector<int> backupLPcount(2,0);

        //backupLPdetailsCount[0][i] = Number of primary LSPs in 'i'th backup LP
        //backupLPdetailsCount[1][i] = Bandwidth usage of primary LSPs in 'i'th backup LP
        vector<vector<int>> backupLPdetailsCount(2,vector<int>()); 

        //Generate sub wavelength graphs for establish LPs
        vector<waveLengthNetworks> subWaveNetworks = setupWaveLengthNetworks(adjacencyMetrix, numOfWaves);

        waveLengthNetworks defaulSubWaveNetworks = subWaveNetworks[0];
        
        lightpathNetwork waveLengthNetwork(subWaveNetworks,backupLPcount,backupLPdetailsCount);
        //lightpathNetwork waveLengthNetwork(subWaveNetworks);
        
        cout << endl << "Generting all LSP requests.\n";
        lspRequestGenarator lspReqGen(numOfNodes);
        lspRequest lspReq;

        LSP lspObj;
        
        vector<int> LPids(100000); // vector with 100000 ints.
        iota(begin(LPids), end(LPids), 1); // Fill with 1, 2, ..., 100000.
        int countForLPids = 100000;
        
        cout << "Establishing and releasing LSP requests.....\n";
        vector<int> pPathFortest;
        vector<int> bPathFortest;
        vector<int> waveVecFortest;

        int count = 0;
        int portion = numberOfLSPrequests / 100;

        while(!listOfEvents.empty())
        {
            
            int vexnum = numOfNodes;
            int source = listOfEvents[0].sourceNode;
            int destination = listOfEvents[0].destinationNode;
            int id = listOfEvents[0].identifier;
            int bandwidth = listOfEvents[0].bandwidth;
            bool action = listOfEvents[0].action;
            
            //Generte a lsp reqest with src,dst,bandwidth, request or remove
            bool isLSPestablish = false;            
           
            

            /* ********************** Check event driven simulation****************************/
            lspPathDetails[id][0].push_back(vector<int>{-1});
            lspPathDetails[id][0].push_back(vector<int>{-1});
            lspPathDetails[id][1].push_back(vector<int>{-1});
            lspPathDetails[id][1].push_back(vector<int>{-1});
            
            
            if(action)
            {
                if(id / portion == count )
                {
                    cout<< count <<"%"<<endl;
                    count++;
                }
                
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
                            waveLengthNetwork.setANewLighpath(moreCovertions.allPathDetailsPP[primaryPart], moreCovertions.wavelengthNumberPP[primaryPart], "pp", LPids[0], lightpathCapacity);
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
                            waveLengthNetwork.setANewLighpath(moreCovertions.allPathDetailsBP[backupPart], moreCovertions.wavelengthNumberBP[backupPart], "pp", LPids[0], lightpathCapacity);
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

                    
                    lspObj.makeLSP(bandwidth, pathForPLSP, wholePathP, wavesP, waveLengthNetwork, "pLSP", id, protectionType, thresholdObj, moreCovertions.allPathDetailsPP);
                    lspObj.makeLSP(bandwidth, pathForBLSP, wholePathB, wavesB, waveLengthNetwork, "bLSP", id, protectionType, thresholdObj, moreCovertions.allPathDetailsBP);

                    isLSPestablish = true;

                    lspPathDetails[id][0][0] = pathForPLSP;
                    lspPathDetails[id][0][1] = wavesP;
                    lspPathDetails[id][1][0] = pathForBLSP;
                    lspPathDetails[id][1][1] = wavesB;

                }

                if (isLSPestablish) lspEstablished++;
                else lspRejected++;
            }
            

            else if(!action)
            {

                 //myfile.writeLog(("New release. Bandwidth = " + to_string(bandwidth) + ",source = " + to_string(source) + ", Dst = "
                   // + to_string(destination) + ", id = " + to_string(id) + ", Release = " + to_string(action)))

                 vector<int> pathP = lspPathDetails[id][0][0];
                 vector<int> wavesP = lspPathDetails[id][0][1];
                 vector<int> pathB = lspPathDetails[id][1][0];
                 vector<int> wavesB = lspPathDetails[id][1][1];

                if (pathP[0] != -1 && wavesP[0] != -1 && pathB[0] != -1 && wavesB[0] != -1)
                {
                    //myfile.writeLog(("New release. Bandwidth = " + to_string(bandwidth) + ",source = " + to_string(source) + ", Dst = "
                       // + to_string(destination) + ", id = " + to_string(id) + ", Release = " + to_string(action)));

                    //lspObj.releaseLSP(pathB, wavesB, waveLengthNetwork, id, thresholdObj, protectionType);
                    lspObj.releaseLSP(pathP, wavesP, waveLengthNetwork, id, thresholdObj, protectionType, "pLSP");
                    lspObj.releaseLSP(pathB, wavesB, waveLengthNetwork, id, thresholdObj, protectionType, "bLSP");

                }

            }

            if (!listOfEvents.empty())
            {
                itr = listOfEvents.begin();
                listOfEvents.erase(itr);
            }
            
        }
        waveLengthNetwork.viewAllLighpaths();

        float totalLSPsInBackupLP = 0;
        float totalBandwidthInBackuLp = 0;

        for(int i = 0; i < backupLPdetailsCount[0].size(); i++)
        {
            totalLSPsInBackupLP += backupLPdetailsCount[0][i];
            totalBandwidthInBackuLp += backupLPdetailsCount[1][i];
        }

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
        myfile.writeLog("Capacity of a LP = " + to_string(lightpathCapacity));
        myfile.writeLog("Lowest capacity of a LSP = " + to_string(lowestLSPcapacity));
        myfile.writeLog("Highest capacity of a LsP = " + to_string(highestLSPcapacity));
        myfile.writeLog("Mean holding time = " + to_string(meanHoldingTime));
        myfile.writeLog("Num.of wave lengths per fiber = "+to_string(numOfWaves));

        myfile.writeLog("                 ");
        myfile.writeLog("****Counts****");
        myfile.writeLog("Num.of lsp rqst = "+to_string(numberOfLSPrequests));
        myfile.writeLog(" ");
        myfile.writeLog("**Num.of lspEstablished LSPs = " + to_string(lspEstablished));
        myfile.writeLog("**Num.of lspRejected LSPs = " + to_string(lspRejected));
        myfile.writeLog(" ");
        myfile.writeLog("Num.of backup LPs established = "+to_string(lpEstablished));
        myfile.writeLog("Num.of backup LPs rejected = "+to_string(lpRejected));
        myfile.writeLog(" ");
        myfile.writeLog("Average num.of LSPs in a backup LP = "+to_string(totalLSPsInBackupLP / backupLPdetailsCount[0].size()));
        myfile.writeLog("Average bandwidth usage in a backup LP = "+to_string(totalBandwidthInBackuLp/ backupLPdetailsCount[1].size() ));      

        
    }


   
    cout << "\nPress ENTER to exit\n";
	cin.get();
    return 0;
}
