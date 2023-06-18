#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <filesystem>
#include <unistd.h>
#include <cstring>
#include "file_utils.h"
#include "mpi.h"
#include <unistd.h>
#include <thread>
#include <semaphore.h>


namespace fs = std::filesystem;


using namespace std;

void removeBreakLine(char* text);

class Chat {

    int myRank;
    int mpiSize;
    string directoryName;
    string chatFileName;
    string inputFileName;
    sem_t semMpiRcv;
    // std::string fileName[30];
public:
    Chat(int myRank, int mpiSize);
    void run();
private:
    void init();
    void tokenRing();
    int getNextProcess();
    int getPreviousProcess();
    void testMpi();
    void checkForMessage();
    void waitForMessage();
    void writeNewMessage(char* message, int sender);
};