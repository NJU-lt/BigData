#include <stdio.h>
#include <mpi.h>
#include <math.h>
// #define N 5
int main(int argc, char** argv) {
    int myid, P, source;
    int C=0;
    int N = 0;
    double SqrtSum=0.0;
    double totalsum = 0.0;
    double starttime = 0.0;
    double endtime = 0.0;
    int numprocs;
    double d;

    MPI_Status status; char message[100];
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    if(myid==0){
        printf("Enter a positive int:");
        fflush(stdout);
        scanf("%d",&N);
        starttime = MPI_Wtime();
    }
    MPI_Bcast(&N,1,MPI_INT,0,MPI_COMM_WORLD);
    int i = myid;
    for (i = myid; i < N; i=i+numprocs) {
            SqrtSum+=sqrt(i*(i+1));
            C++;
        }
    MPI_Reduce(&SqrtSum,&totalsum,1,MPI_DOUBLE,MPI_SUM, 0, MPI_COMM_WORLD);
    if(myid==0) 
    { 
        printf("I am process 0. final result is %f.\n",totalsum);
        endtime=MPI_Wtime();
        printf("time=%f.\n",endtime-starttime);
    }

    printf("I am process %d. I calculate total %d ,and SqrtSum=%f.\n", myid, C, SqrtSum);
    MPI_Finalize();
return 0;
}