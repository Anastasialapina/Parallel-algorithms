#include <stdio.h>
#include "stdlib.h"
#include <mpi.h>
#include "time.h"
#include "math.h"

void main( int argc, char *argv[] ) {
    srand(time(NULL));
    int N = atoi(argv[1]);
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
    MPI_Status Status;

    //перессылка N всем процессам
    if ( ProcRank == 0 ){
        for (int i = 1; i<ProcNum; i++ ) {
            MPI_Recv(&N, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
        }
    }
    else
        MPI_Send(&N,1,MPI_INT,0,0, MPI_COMM_WORLD);

    int Dims[] = {N, round(ProcNum/N)}, Periods[] = {0, 0};
    MPI_Cart_create(MPI_COMM_WORLD, 2, Dims, Periods, 0, &Comm);
    int Coords[2];
    if(ProcRank < (N*round(ProcNum/N))){
        MPI_Cart_coords(Comm, ProcRank, 2, Coords);
        printf("Process = %d -> (%d; %d)\n", ProcRank, Coords[0], Coords[1]);
    }

    if(ProcRank == 0){
        double Finish = MPI_Wtime();
        double Time = Finish-Start;
        printf("Time: %f\n", Time);
    }
    MPI_Finalize();
}

