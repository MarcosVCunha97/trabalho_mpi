#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> //necessário p/ função time()
#include <math.h>
#include "mpi.h"

int numero, myrank, size;

int getNextProcess();
int getPreviousProcess();
double t1, t2;

int main(int argc, char **argv) {
    MPI_Status status;
    MPI_Init (&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    printf("Ola... \n");
    if(myrank == 0) {
        printf("Sou o processo 0 \n");
        scanf("%d", &numero);
        t1 = MPI_Wtime();
        MPI_Send(&numero,1,MPI_INT,getNextProcess(),99,MPI_COMM_WORLD);
        MPI_Recv(&numero,1,MPI_INT,getPreviousProcess(),99,MPI_COMM_WORLD,&status);
        printf("Sou o processo 0, recebi do processo %d o valor %d \n", getPreviousProcess(),numero);
        t2 = MPI_Wtime();
        printf("\nTempo: %f\n", t2-t1);
    }
    else{
        char name[MPI_MAX_PROCESSOR_NAME];
        int resultlength;
        MPI_Get_processor_name(name, &resultlength);
        printf("Eu sou o processo %s \n", name);
        MPI_Recv(&numero,sizeof(int),MPI_INT,getPreviousProcess(),99,MPI_COMM_WORLD,&status);
        numero = numero + 10 ;
        MPI_Send(&numero,1,MPI_INT,getNextProcess(),99,MPI_COMM_WORLD);
    }
    MPI_Finalize();
}

int getNextProcess(){
    return ( myrank + 1 ) % size;
}

int getPreviousProcess(){
    return ( myrank + size - 1 ) % size;
}