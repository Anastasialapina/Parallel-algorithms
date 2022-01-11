#include <stdio.h>
#include "stdlib.h"
#include <mpi.h>
#include "time.h"

void main( int argc, char *argv[] ) {
    srand(time(NULL));
    int i, ProcRank, ProcNum, RecvRank;
    MPI_Init(&argc, &argv);
    MPI_Status Status;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    int Size = 3 * ProcNum;
    int array[Size];
    double Start = MPI_Wtime();
    int SendArr[3];
    if (ProcRank == 0){
        for (i = 0; i < Size; i++){
            array[i] = rand() % 100;
            printf("%d  ", array[i]);
            if (i == Size - 1)
                printf("\n");
        }
//        double Finish = MPI_Wtime();
//        double Time = Finish-Start;
//        printf("Time: %f\n", Time);
    }

    MPI_Scatter(array, 3, MPI_INT, SendArr, 3, MPI_INT, 0, MPI_COMM_WORLD);
    printf("Array by process %d: ", ProcRank);
    for(i = 0; i<3; i++){
        printf("%d ", SendArr[i]);
    }
    printf("\n");
    if(ProcRank == ProcNum-1){
        double Finish = MPI_Wtime();
        double Time = Finish-Start;
        printf("Time: %f\n", Time);
    }
    MPI_Finalize();
}


