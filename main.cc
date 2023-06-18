#include "chat.h"
#include "file_utils.h"
#include "mpi.h"

using namespace std;


int main(int argc, char **argv){
    int numero, myrank, size;

    MPI_Init (&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    Chat chat = Chat(myrank, size);
    chat.run();

    MPI_Finalize();

    // char directoryName[] = "meu_diretorio/teste.txt";
    // char* c = readFromFile(directoryName);
    
    // cout << "O arquivo está vazio? " << (c[0] == '\0') << endl;
    // chat.createFile(directoryName);
    // char* c = readLastLine(directoryName);
    // chat.eraseLastLine(directoryName);
    // cout << c << endl;
}
