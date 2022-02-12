#pragma once

bool protectionType = true;              //True for bandwidth based LP protection. False for number of LSPs based LP protection
bool hybrid = true;
float bandwidthThreshold = 0.5;  //Assigning the threshold values
int numLSPthreshold = 1;        //Assigning the threshold values
int numberOfLSPrequests = 3000;           //The number of LSP requests
double erlang = 20;                      //Erlang value
double meanHoldingTime = 1;              //Mean holding time
int numOfWaves = 16;
int bandwidthCap = 100;
//int numberOfNodesForDefault = 14;


bool writeEventToAFile = true;
string writeEventToAFilePath = "rqst_inputs/rq_test_2022_02_12.txt";
bool readEventFromAFile = !writeEventToAFile;
string readEventFromAFilePath = "rqst_inputs/rq_test_2022_02_12.txt";

string CsvFileLocation = "C:/Users/ASUS/source/repos/FYP/FYP/graph_inputs/05/graph05.csv";
string logFileLocation = "C:/Users/ASUS/source/repos/FYP/FYP/log_files/";