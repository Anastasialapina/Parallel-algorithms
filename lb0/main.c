#include <stdio.h>
#include "mpi.h"
int main(int argc, char* argv[]){
    int Size = 100000;
    int SendData[Size];
    int RecvData[Size];
    double SumTime = 0;
    int ProcNum, ProcRank, RecvRank;
    MPI_Status Status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    SendData[0] = ProcRank;
    for (int i = 1; i < Size; i++) {
        SendData[i] = i;
    }
    double Start, Finish;
    Start = MPI_Wtime();
    if (ProcRank % 2 == 0){
        if (ProcRank +1 < ProcNum) {
            MPI_Send(&SendData, Size, MPI_INT, ProcRank + 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&RecvData, Size, MPI_INT, ProcRank + 1, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
            printf ("Process %d -> process %d\n", ProcRank, RecvData[0]);
        }
    } else {
        MPI_Recv(&RecvData, Size, MPI_INT, ProcRank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
        MPI_Send(&SendData, Size, MPI_INT, ProcRank - 1, 0, MPI_COMM_WORLD);
        printf ("Process %d -> process %d\n", ProcRank, RecvData[0]);
    }

    Finish = MPI_Wtime();
    double Time = Finish-Start;
    MPI_Reduce(&Time, &SumTime, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    if (ProcRank == 0) {
        printf("Average time: %f\n", SumTime/ProcNum);
    }
    MPI_Finalize();
    return 0;
}