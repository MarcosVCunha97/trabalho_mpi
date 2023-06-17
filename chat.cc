#include "chat.h"


Chat::Chat(int myRank, int mpiSize) {
    this->myRank = myRank;
    this->mpiSize = mpiSize;
}

/// Cria o diretório de acordo com o id do processo
/// Cria o arquivo de chat
void Chat::init() {
    // proc_ID: ex proc_5
    this->directoryName = "proc_" + to_string(this->myRank);
    this->chatFileName = this->directoryName + "/chat.txt";
    this->inputFileName = this->directoryName + "/input.txt";
    createDirectory((char *)directoryName.c_str());
    createFile((char *)chatFileName.c_str());
}

void Chat::tokenRing() {
    MPI_Status status;
    int numero = 0;

    if (this->myRank == 0) {
        createFile((char *)inputFileName.c_str());
        usleep(3000000);
        deleteFile((char *)inputFileName.c_str());
        MPI_Send(&numero,1,MPI_INT,this->getNextProcess(),99,MPI_COMM_WORLD);
        MPI_Recv(&numero,1,MPI_INT,this->getPreviousProcess(),99,MPI_COMM_WORLD,&status);
        createFile((char *)inputFileName.c_str());
        usleep(3000000);
        deleteFile((char *)inputFileName.c_str());
        cout << "Processo " << this->myRank << " terminou" << endl;
    }
    else{
        MPI_Recv(&numero,sizeof(int),MPI_INT,this->getPreviousProcess(),99,MPI_COMM_WORLD,&status);
        createFile((char *)inputFileName.c_str());
        usleep(3000000);
        deleteFile((char *)inputFileName.c_str());
        MPI_Send(&numero,1,MPI_INT,this->getNextProcess(),99,MPI_COMM_WORLD);
    }
}

int Chat::getNextProcess() {
    return (this->myRank + 1) % this->mpiSize;
}

int Chat::getPreviousProcess() {
    return (this->myRank - 1 + this->mpiSize) % this->mpiSize;
}

// int getNextProcess(){
//     return ( myrank + 1 + ) % size;
// }

// int getPreviousProcess(){
//     return ( myrank + size - 1 ) % size;
// }