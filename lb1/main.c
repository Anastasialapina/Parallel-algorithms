#include <stdio.h>
#include "stdlib.h"
#include <mpi.h>
#include "time.h"

struct{
    int value;
    int proc;
}local_max,global_max;

void main( int argc, char *argv[] ) {
    srand(time(NULL));
    int i, ProcRank, ProcNum, RecvRank;
    int Size = 10;
    MPI_Init(&argc, &argv);
    MPI_Status Status;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    int array[Size];
    int max_value;
    double Start = MPI_Wtime();
    if (ProcRank == 0){
        for (i = 0; i < Size; i++){
            array[i] = rand() % 100;
            printf("%d  ", array[i]);
            if (i == Size - 1)
                printf("\n");
        }
        for (i = 1; i < ProcNum; i++){
            MPI_Send(&array, Size, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }
    else{
        MPI_Recv(&array, Size, MPI_INT, 0, 0, MPI_COMM_WORLD, &Status);
        max_value=array[0];
        for(i=0; i<Size; i++){
            if(array[i]>max_value)
                max_value=array[i];
        }
        local_max.value = max_value;
    }
    MPI_Reduce(&local_max, &global_max, 1, MPI_2INT, MPI_MAXLOC, ProcNum - 1, MPI_COMM_WORLD );
    double Finish = MPI_Wtime();
    double Time = Finish-Start;
    if (ProcRank == ProcNum - 1){
        printf("Global maximum = %d\n", global_max.value);
        printf("Time: %f\n", Time);
    }
    MPI_Finalize();
}


