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
        //physicalLinkNetwork.printGraph();

        vector<waveLengthNetworks> subWaveNetworks = setupWaveLengthNetworks(adjacencyMetrix, 40);

        waveLengthNetworks defaulSubWaveNetworks = subWaveNetworks[0];
        
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
        cout << createRemaingDeatils.canCreatRemainPP << endl; // ithiri hariyata hadanna puluwanda baida kiyala PP
        cout << createRemaingDeatils.wavelengthRemainigPPNo << endl; // wavelenth Number PP
        printShortestPath(createRemaingDeatils.wavelengthRemainigPP); // print path PP
        cout << createRemaingDeatils.canCreatRemainBP << endl; // ithiri hariyata hadanna puluwanda baida kiyala BP
        cout << createRemaingDeatils.wavelengthRemainigBPNo << endl; // wavelenth Number BP
        printShortestPath(createRemaingDeatils.wavelengthRemainigBP); // print path BP
        cout << "555555555555555555555555555555555555555555555555555555555555555555555555" << endl;

        //From leni combine two wavelength
        combineWavelength combineWavelengthDetails = pathCombinationCreat(vexnum, subWaveNetworks,  source,  destination);
        cout << "444444444444444444444444444444444444444444444444444444444444444444444444" << endl;
        cout << combineWavelengthDetails.canCreatCombinationPP << endl; //wavelength dekak connect karanna puluwanda baida kiyala balanawa PP
        cout << combineWavelengthDetails.wavelengthNo1PP << endl; //from source wavlength number PP
        cout << combineWavelengthDetails.wavelengthNo2PP << endl; //to destination wavlength number PP
        cout << combineWavelengthDetails.connectingNodePP << endl; // primary wavelength intermediate node
        printShortestPath(combineWavelengthDetails.w1ShortPathPP); // print path from source PP
        printShortestPath(combineWavelengthDetails.w2ShortPathPP); // print from intermediate node PP
        cout << combineWavelengthDetails.canCreatCombinationBP << endl; //wavelength dekak connect karanna puluwanda baida kiyala balanawa BP
        cout << combineWavelengthDetails.wavelengthNo1BP << endl; //from source wavlength number BP
        cout << combineWavelengthDetails.wavelengthNo2BP << endl; //to destination wavlength number BP
        cout << combineWavelengthDetails.connectingNodeBP << endl; // primary wavelength intermediate node BP
        printShortestPath(combineWavelengthDetails.w1ShortPathBP); // print path from source BP
        printShortestPath(combineWavelengthDetails.w2ShortPathBP); // print from intermediate node BP
        cout << "444444444444444444444444444444444444444444444444444444444444444444444444" << endl;

        //From Leni Create BackupLight Path


        /*




        vector<int> heavylightpath; // Meka Bauplight path Vector eka

        forBackupLightpath createLightPathBackupDeatls =  createLightPathBackup( vexnum, heavylightpath, subWaveNetworks,  source,  destination);
        cout << "222222222222222222222222222222222222222222222222222222222222222222222222" << endl;
        cout << createLightPathBackupDeatls.canCreatBLPath << endl; //BackupLight path hadanna puluwanda baida kiyala
        cout << createLightPathBackupDeatls.wavelengthBLPNo << endl; // wavelenth Number
        printShortestPath(createLightPathBackupDeatls.wavelengthBLPath); // print path
        cout << "22222222222222222222222222222222222222222222222222222222222222222222222222222222222" << endl;





        /*

        /*******************************************************************************************************************/
        
        for(int i = 0; i < 1; )
        {

            //Generte a lsp reqest with src,dst,bandwidth, request or remove
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
                                if(createRemaingDeatils.canCreatRemainPath)
                                {
                                    waveLengthNetwork.setANewLighpath(createRemaingDeatils.wavelengthRemainigPath,"14");
                                    pathDetails.canCreatPP = true;
                                    break;
                                }
                                


                            }
                        }

                        combineWavelength combineWavelengthDetails = pathCombinationCreat(vexnum, subWaveNetworks,  source,  destination);
                        
                        //If there is no any LP in shortest path or can not create using 2 LPs
                        //Try to find path using any 2 LPs (not within shortest path)
                        if(combineWavelengthDetails.canCreatCombination)
                        {
                            int wave1 = combineWavelengthDetails.wavelengthNo1;
                            int wave2 = combineWavelengthDetails.wavelengthNo2;

                            vector<int> temp = {combineWavelengthDetails.w1ShortPath[0],
                                                combineWavelengthDetails.w2ShortPath[0],
                                                combineWavelengthDetails.w2ShortPath[combineWavelengthDetails.w2ShortPath.size()-1]};

                            //waveLengthNetwork.setANewLSP(temp," ",waveLengthNetwork);
                            pathDetails.canCreatPP = true;
                        }

                        myfile.writeLog("Primary LSP path can not establish for this request.");
                        //break;
                    }

                    if(!pathDetails.canCreatBP && pathDetails.canCreatPP)
                    {
                        myfile.writeLog("Backup LSP path can not establish for this request.");
                        //break;
                    }

                    //If using new LPs can establish path for LSP reqeust
                    if(pathDetails.canCreatBP && pathDetails.canCreatPP)
                    {
                        waveLengthNetwork.setANewLighpath(pathDetails.primaryShortPath,to_string(pathDetails.wavelengthNoPP) );
                        waveLengthNetwork.setANewLighpath(pathDetails.backUpShortPath,to_string(pathDetails.wavelengthNoBP));

                        adjMetrixForPrimaryLSP = waveLengthNetwork.lpPAdjacencyMetrix(lspReq.bandwidthSize, numOfNodes);
                       
                        pathDetails = startingPoint(vexnum, subWaveNetworks, source, destination,adjMetrixForPrimaryLSP);
                        myfile.writeLog("new LP is established");
                        
                        if (pathDetails.alreadyPPhave == false) 
                        {
                            //Creat LSP here
                            vector<int> temp = {pathDetails.tempPrimaryShortPath[0],pathDetails.tempPrimaryShortPath[pathDetails.tempPrimaryShortPath.size()-1]};
                            //waveLengthNetwork.setANewLSP(temp,"4",waveLengthNetwork);
                        }
                        cout<<"\nFrom else\n";
                        cout << "Primary path can create  --->  " << pathDetails.tempCanCreatPP << endl; //true or false
                        cout << "Primary path wavelength No --->  " << pathDetails.tempWavelengthNoPP << endl;
                        cout << "Primary path" << endl;
                        printShortestPath(pathDetails.tempPrimaryShortPath); //print temparary shortest path
                        cout << "vector Size ----> " << pathDetails.tempPrimaryShortPath.size() << endl;
                    }
                }

                //Already there is path(LP) for LSP request
                if(pathDetails.alreadyPPhave == true) 
                {
                    //Create LSP here
                    vector<int> temp = {pathDetails.tempPrimaryShortPath[0],pathDetails.tempPrimaryShortPath[pathDetails.tempPrimaryShortPath.size()-1]};
                   // waveLengthNetwork.setANewLSP(pathDetails.tempPrimaryShortPath,"4",waveLengthNetwork);

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
        cout<<endl<<"\n**NetWork**\n";
        waveLengthNetwork.viewAllLighpaths();
        

    }
    
	cin.get();
    return 0;
}
