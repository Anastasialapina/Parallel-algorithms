#include <stdio.h>
#include "stdlib.h"
#include <mpi.h>
#include "time.h"

struct struct_{
    int N;
    float A;
};

void main( int argc, char *argv[] ) {
    srand(time(NULL));
    float Sum = 0;
//    struct struct_ array[4] = {{0}, {1, 0.7}, {1, 0.3}, {0}};
    struct struct_ array[2] = {{0}, {1, 0.555}};
//    struct struct_ array[6] = {{0}, {1, 0.111}, {1, 0.3}, {0}, {1, 0.9}, {0}};
//    struct struct_ array[8] = {{1, 0.33}, {0}, {0}, {0}, {1, 0.11}, {0}, {1, 0.22}, {0}};
    int ProcRank, ProcNum;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

    int Flag;
    MPI_Initialized(&Flag);
    if (Flag == 0)
        return;

    double Start = MPI_Wtime();
    MPI_Comm Comm;
    int Color;
    if(array[ProcRank].N == 1)
        Color = 0;
    else
        Color = MPI_UNDEFINED;
    MPI_Comm_split(MPI_COMM_WORLD, Color, ProcRank, &Comm);
    if (Comm == MPI_COMM_NULL)
        return;

    int NewProcRank;
    MPI_Comm_size(Comm, &ProcNum);
    MPI_Comm_rank(Comm, &NewProcRank);

    MPI_Reduce(&array[ProcRank].A, &Sum, 1, MPI_FLOAT, MPI_SUM, 0, Comm);

    if(NewProcRank == 0){
        double Finish = MPI_Wtime();
        double Time = Finish-Start;
        printf("Sum = %f\n", Sum);
        printf("Time: %f\n", Time);
    }
    MPI_Finalize();
}

