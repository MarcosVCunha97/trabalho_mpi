#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <filesystem>
#include <unistd.h>
#include <cstring>
#include "file_utils.h"
#include "mpi.h"
#include<unistd.h>


namespace fs = std::filesystem;


using namespace std;

void removeBreakLine(char* text);

class Chat {

    int myRank;
    int mpiSize;
    string directoryName;
    string chatFileName;
    string inputFileName;
    // std::string fileName[30];
public:
    Chat(int myRank, int mpiSize);
    void init();
    void tokenRing();
private:
    int getNextProcess();
    int getPreviousProcess();
};