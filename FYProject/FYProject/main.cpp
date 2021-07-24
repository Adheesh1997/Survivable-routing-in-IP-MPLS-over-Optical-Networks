#include <iostream>
#include <string>
#include <vector>

#include "files.h"
#include "graph.h"
#include "lightpathSettingup.h"
#include "Dijkstra.h"
#include "lspRequestGenarator.h"

using namespace std;

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
        
        for(int i = 0; i < 1000; )
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
                
                /* cout<<"\nsource_ ";
                cin>>source;
                cout<<"\ndestination_ ";
                cin>>destination; */


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

                                        waveLengthNetwork.setANewLSP(pathForLSP,primaryWaves,waveLengthNetwork,"pp",lspReq.id);
                                        waveLengthNetwork.setANewLSP(pathForLSP,backupWaves,waveLengthNetwork,"bp",lspReq.id);

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
                            waveLengthNetwork.setANewLSP(pathForPLSP,wavesP,waveLengthNetwork,"pp",lspReq.id);

                            waveLengthNetwork.setANewLighpath(combineWavelengthDetails.w1ShortPathBP, combineWavelengthDetails.wavelengthNo1BP,"pp");
                            waveLengthNetwork.setANewLighpath(combineWavelengthDetails.w2ShortPathBP, combineWavelengthDetails.wavelengthNo2BP, "pp");

                            vector<int> pathForBLSP = {combineWavelengthDetails.w1ShortPathBP[0],combineWavelengthDetails.connectingNodeBP,combineWavelengthDetails.w2ShortPathBP[combineWavelengthDetails.w2ShortPathBP.size()-1]};
                            vector<int> wavesB = {combineWavelengthDetails.wavelengthNo1BP, combineWavelengthDetails.wavelengthNo2BP};

                            waveLengthNetwork.setANewLSP(pathForBLSP,wavesB,waveLengthNetwork,"bp",lspReq.id);

                            myfile.writeLog("New lsp establish with 2 new LPs.**");
                            new2LPs++;
                            isLSPestablish = true;

                        }
                    }

                    /* if(!pathDetails.canCreatBP && pathDetails.canCreatPP && !isLSPestablish)
                    {
                        myfile.writeLog("Backup LSP path can not establish for this request.");
                        //break;
                    } */

                    //If using new LPs can establish path for LSP reqeust
                    if(pathDetails.canCreatBP && pathDetails.canCreatPP && !isLSPestablish)
                    {
                        waveLengthNetwork.setANewLighpath(pathDetails.primaryShortPath, pathDetails.wavelengthNoPP,"pp" );
                        waveLengthNetwork.setANewLighpath(pathDetails.backUpShortPath,pathDetails.wavelengthNoBP,"pp");

                        /* cout<<"\n\n**Primary path***\n";
                        printShortestPath(pathDetails.primaryShortPath);
                        cout<<"\nWave no P: "<<pathDetails.wavelengthNoPP<<endl;
                        cout<<"\n\n**backup path***\n";
                        printShortestPath(pathDetails.backUpShortPath);
                        cout<<"\nWave no B: "<<pathDetails.wavelengthNoBP<<endl; */

                        vector<int> pathForPLSP = {pathDetails.primaryShortPath[0],pathDetails.primaryShortPath[pathDetails.primaryShortPath.size()-1]};
                        vector<int> wavesP = {pathDetails.wavelengthNoPP};
                        waveLengthNetwork.setANewLSP(pathForPLSP,wavesP,waveLengthNetwork,"pp",lspReq.id);

                        vector<int> pathForBLSP = {pathDetails.backUpShortPath[0],pathDetails.backUpShortPath[pathDetails.backUpShortPath.size()-1]};
                        vector<int> wavesB = {pathDetails.wavelengthNoBP};
                        waveLengthNetwork.setANewLSP(pathForBLSP,wavesB,waveLengthNetwork,"bp",lspReq.id);

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

                                waveLengthNetwork.setANewLSP(pathForLSP,wavesP,waveLengthNetwork,"pp",lspReq.id);
                                waveLengthNetwork.setANewLSP(pathForLSP,wavesB,waveLengthNetwork,"bp",lspReq.id);

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

                            waveLengthNetwork.setANewLSP(pathForLSP,wavesP,waveLengthNetwork,"pp",lspReq.id);
                            waveLengthNetwork.setANewLSP(pathForLSP,wavesB,waveLengthNetwork,"bp",lspReq.id);
                            
                            myfile.writeLog("New lsp established with 2 old LPs.");
                            oldLP++;
                            isLSPestablish = true;
                        }
                    }                
                }
                
                if(!isLSPestablish) 
                {
                    rejected++;
                    myfile.writeLog("LSP is REJECTED.");
                }
                
                
            }

        }
       
        /* myfile.writeLog("\n****Counts****");
        myfile.writeLog("new LP & old LP = "+to_string(newLPnoldLP));
        myfile.writeLog("new 2 LPs  = "+to_string(new2LPs));
        myfile.writeLog("new LP = "+to_string(newLP));
        myfile.writeLog("old LP = "+to_string(oldLP));
        myfile.writeLog("\nNum.of LSP established = "+to_string(newLPnoldLP+new2LPs+newLP+oldLP));
        myfile.writeLog("Num.of LSP rejected = "+to_string(rejected)); */
    }
    
	cin.get();
    return 0;
}
