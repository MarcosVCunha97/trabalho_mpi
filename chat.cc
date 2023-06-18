#include "chat.h"
#define WAIT_TIME 10000000
#define MESSAGE_DELAY_TIME 3000000
#define MAX_MESSAGE_SIZE 200
#define TAG_TOKEN 99
#define TAG_BLOCK_FILE 98
#define TAG_MESSAGE 100

Chat::Chat(int myRank, int mpiSize) {
    this->myRank = myRank;
    this->mpiSize = mpiSize;
}

void Chat::run() {
    this->init();
    // this->testMpi();

    thread t1(&Chat::tokenRing, this);
    thread t2(&Chat::waitForMessage, this);
    t1.join();    
    t2.join();
}

void Chat::testMpi(){
    int num = 0;
    if(this->myRank == 0){
        num = 42;
        for(int i = 1; i < this->mpiSize; i++){
            MPI_Send(&num, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
        }
    } else {
        
        MPI_Recv(&num, 1, MPI_INT, 0, this->myRank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        cout << "Processo " << this->myRank << " recebeu o token" << endl;
    }
}

/// Cria o diretório de acordo com o id do processo
/// Cria o arquivo de chat
void Chat::init() {
    // proc_ID: ex proc_5
    
    this->directoryName = "proc_" + to_string(this->myRank);
    this->chatFileName = this->directoryName + "/chat.txt";
    this->inputFileName = this->directoryName + "/input.txt";
    createDirectory((char *)directoryName.c_str());
    deleteFile((char *)chatFileName.c_str());
    createFile((char *)chatFileName.c_str());
    deleteFile((char *)inputFileName.c_str());
    // sem_init(&this->semMpiRcv, 0, 1);
}

void Chat::tokenRing() {
    MPI_Status status;
    int numero = 0;
    int i = 0;
    while(i < 3){
        if (this->myRank == 0) {
            checkForMessage();
            MPI_Send(&numero,1,MPI_INT,this->getNextProcess(),TAG_TOKEN,MPI_COMM_WORLD);
            cout << "Processo: " << this->myRank << " enviou o token para o processo: " << this->getNextProcess() << endl;
            MPI_Recv(&numero,1,MPI_INT,this->getPreviousProcess(),TAG_TOKEN,MPI_COMM_WORLD,&status);
            cout << "Processo: " << this->myRank << " recebeu o token do processo: " << this->getPreviousProcess() << endl;
        }
        else{
            MPI_Recv(&numero,1,MPI_INT,this->getPreviousProcess(),TAG_TOKEN,MPI_COMM_WORLD,&status);
            cout << "Processo: " << this->myRank << " recebeu o token do processo: " << this->getPreviousProcess() << endl;
            checkForMessage();
            MPI_Send(&numero,1,MPI_INT,this->getNextProcess(),TAG_TOKEN,MPI_COMM_WORLD);
            cout << "Processo: " << this->myRank << " enviou o token para o processo: " << this->getNextProcess() << endl;
        }
        i++;
    }
}

void Chat::checkForMessage() {
    createFile((char *)inputFileName.c_str());
    usleep(WAIT_TIME);

    char* content = readFromFile((char *)inputFileName.c_str());
    // char data[MAX_MESSAGE_SIZE];
    char* data = new char[MAX_MESSAGE_SIZE];

    strcpy(data, content);

    if(content != NULL && content[0] != '\0'){
        char* chatContent = readFromFile((char *)chatFileName.c_str());
        deleteFile((char *)chatFileName.c_str());
        for(int rank = 0; rank < this->mpiSize; rank++){
            int num = this->myRank;
            if(rank != this->myRank){
                MPI_Send(&num, 1, MPI_INT, rank, TAG_BLOCK_FILE, MPI_COMM_WORLD);
            }
        }
        usleep(MESSAGE_DELAY_TIME);
        for(int rank = 0; rank < this->mpiSize; rank++){
            if(rank != this->myRank){
                MPI_Send(data, MAX_MESSAGE_SIZE, MPI_CHAR, rank, TAG_MESSAGE, MPI_COMM_WORLD);
            }
        }
        createFile((char *)chatFileName.c_str());
        writeToFile((char *)chatFileName.c_str(), chatContent);
        this->writeNewMessage(data, this->myRank);
    }
    deleteFile((char *)inputFileName.c_str());
}

void Chat::waitForMessage() {
    char* text = new char[MAX_MESSAGE_SIZE];
    MPI_Status status;
    while(true){
        int num;
        cout << "Processo " << this->myRank << " esperando mensagem" << endl;
        MPI_Recv(&num, 1, MPI_INT, MPI_ANY_SOURCE, TAG_BLOCK_FILE, MPI_COMM_WORLD, &status);

        cout << "Processo " << this->myRank << " bloqueado" << endl;
        char* chatContent = readFromFile((char *)chatFileName.c_str());
        deleteFile((char *)chatFileName.c_str());
        usleep(50000);
        MPI_Recv(text, MAX_MESSAGE_SIZE, MPI_CHAR, MPI_ANY_SOURCE, TAG_MESSAGE, MPI_COMM_WORLD, &status);
        int source = status.MPI_SOURCE;
        int tag = status.MPI_TAG;

        cout << "Processo " << this->myRank << " recebeu a mensagem do proc: " << source << " com a tag: " << tag << endl;
        createFile((char *)chatFileName.c_str());
        writeToFile((char *)chatFileName.c_str(), chatContent);
        this->writeNewMessage(text, source);
        usleep(500000);
    }
}

void Chat::writeNewMessage(char* message, int sender) {
    string messageHeader = "MENSAGEM - Processo " + to_string(sender) + ":\n\n";
    char endMessage[4];
    strcpy(endMessage, "\n\n\n");
    writeToFile((char *)chatFileName.c_str(), (char *)messageHeader.c_str());
    writeToFile((char *)chatFileName.c_str(), message);
    writeToFile((char *)chatFileName.c_str(), endMessage);
    cout << "Processo " << this->myRank << " recebeu a mensagem: " << message << endl;
}

int Chat::getNextProcess() {
    return (this->myRank + 1) % this->mpiSize;
}

int Chat::getPreviousProcess() {
    return (this->myRank - 1 + this->mpiSize) % this->mpiSize;
}