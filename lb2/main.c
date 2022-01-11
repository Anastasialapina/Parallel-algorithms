#include <stdio.h>
#include "stdlib.h"
#include <mpi.h>
#include "time.h"
#include <math.h>

struct{
    int value;
    int proc;
}local_sum,global_sum;

void main( int argc, char *argv[] ) {
    srand(time(NULL));
    int koef = atoi(argv[1]);
    int i, ProcRank, ProcNum, RecvRank;
    MPI_Init(&argc, &argv);
    MPI_Status Status;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    i = 1;
    int Size = 0;
    for(i = 1; i < ProcNum; i++){
        Size = Size + (1<<i);
    }
    Size = Size * koef; //размер массива с данными
    int array[Size];
    int NewSize;
    double Start = MPI_Wtime();
    if (ProcRank == 0){
        int OrderArr[ProcNum]; //массив для определения процесса для отправки данных 0 процессом
        for(i = 0; i < ProcNum; i++){
            OrderArr[i] = i;
        }
        //для отправки рандомному процессу
        for(int i = 1; i<ProcNum; i++){
            int ind1 = 1 + (rand() % (ProcNum-1));
            int ind2 = 1 + (rand() % (ProcNum-1));
            int temp =  OrderArr[ind1];
            OrderArr[ind1] = OrderArr[ind2];
            OrderArr[ind2] = temp;
        }
        //заполняем массив рандомными числами
        for (i = 0; i < Size; i++){
            array[i] = rand() % 100;
            printf("%d  ", array[i]);
            if (i == Size - 1)
                printf("\n");
        }
        NewSize = Size/2 + 1;
        int j_NewArr = 0;
        for (i = 1; i < ProcNum; i++){
            int SendArr[NewSize];
            for(int j = 0; j < NewSize; j++){
                SendArr[j] = array[j_NewArr];
                j_NewArr++;
            }
            //передаем другим процессам количество элементов в подмассиве
            MPI_Send(&NewSize, 1, MPI_INT, OrderArr[i], 0, MPI_COMM_WORLD);
            //передаем другим процессам массив по частям
            MPI_Send(&SendArr, Size, MPI_INT, OrderArr[i], 0, MPI_COMM_WORLD);
            NewSize = NewSize/2;
            //принимаем результат работы от процессов
            MPI_Recv(&local_sum.value, 1, MPI_INT, OrderArr[i], 0, MPI_COMM_WORLD, &Status);
            //обрабатываем полученный результат
            global_sum.value = global_sum.value + local_sum.value;
            double Finish = MPI_Wtime();
            double Time = Finish-Start;
            if (i == ProcNum - 1){
                printf("Sum = %d\n", global_sum.value);
                printf("Time: %f\n", Time);
            }
         }
    }
    else{
        //принимаем массив от 0 процесса
        MPI_Recv(&NewSize, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &Status);
        int RecvArr[NewSize];
        MPI_Recv(&RecvArr, Size, MPI_INT, 0, 0, MPI_COMM_WORLD, &Status);
        //ищем сумму подмассива
        local_sum.value = 0;
        for(i=0; i<NewSize; i++){
            local_sum.value = local_sum.value + RecvArr[i];
        }
        //отправляем обработку в нулевой процесс
        MPI_Send(&local_sum.value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();
}


