#pragma once

bool protectionType = false;              //True for bandwidth based LP protection. False for number of LSPs based LP protection
bool hybrid = false;
float bandwidthThreshold = 0.5;  //Assigning the threshold values
int numLSPthreshold = 1;        //Assigning the threshold values
int numberOfLSPrequests = 500;           //The number of LSP requests
double erlang = 70;                      //Erlang value
double meanHoldingTime = 1;              //Mean holding time
int numOfWaves = 16;
int bandwidthCap = 100;


bool writeEventToAFile = false;
string writeEventToAFilePath = "rqst_inputs/rq_test_2022_01_19.txt";
bool readEventFromAFile = !writeEventToAFile;
string readEventFromAFilePath = "rqst_inputs/rq_test_2022_01_19.txt";

string CsvFileLocation = "graph_inputs/05/graph05.csv";
string logFileLocation = "log_files/";