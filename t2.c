#define N 10000000
#define a 10
#define b 100
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
int main(int argc, char** argv) {
int myid,numprocs;
int i,t;

double local=0.0, dx=(double)(b-a)/N; /* 小矩形宽度 */ 
double inte = 0.0; //最后结果
double x; 
double starttime=0.0,endtime=0.0; //记录运行时间

MPI_Init(&argc, &argv);
MPI_Status status;
MPI_Comm_rank(MPI_COMM_WORLD, &myid);
MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
--numprocs;

if(myid == 0){
    
    starttime=MPI_Wtime();
    
    for(i = 0;i<N;i++)
		MPI_Send(&i,1,MPI_INT,i%numprocs+1,1,MPI_COMM_WORLD);
    int source = 1;
    for (source = 1; source <= numprocs; source++) /*结果收集*/{
        double d;
        MPI_Recv(&d, 1, MPI_DOUBLE, source, 99, MPI_COMM_WORLD, &status); 
        inte += d; 
    }
}
else{
    int j;
    for(j=myid-1;j<N;j=j+numprocs) /* 根据节点数目将N个矩形分为图示的多个颜色组 */{/* 每个节点计算一个颜色组的矩形面积并累加*/
        int t;
        MPI_Recv(&t, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status); 
        x = a + t*dx +dx/2; /* 以每个矩形的中心点x值计算矩形高度 */
        local +=x*x*x*dx;
        
        }
        MPI_Send(&local, 1, MPI_DOUBLE, 0, 99, MPI_COMM_WORLD);
}

if(myid==0) 
{ 
    printf("The integal of x*x*x in region [%d,%d] =%16.15f\n", a, b, inte);
    endtime=MPI_Wtime();
	printf("time=%f.\n",endtime-starttime);
}
int namelen;
char processor_name[MPI_MAX_PROCESSOR_NAME];
MPI_Get_processor_name(processor_name,&namelen);
printf("I am process %d on %s . sum =%16.15f.\n",myid,processor_name,local);
MPI_Finalize();
}
