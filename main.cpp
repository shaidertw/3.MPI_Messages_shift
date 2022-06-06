#include <iostream>

#include "mpi.h"
#include "omp.h"

int main(int argc, char* argv[]){

	int arrSize = 10;

	for (int i = 0; i < argc; i++){
		if (strncmp(argv[i], "--size-array", 12) == 0){
			arrSize = atoi(argv[i + 1]);
			break;
		}
	}

	MPI_Init(NULL, NULL);
	int rank, total;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &total);
	MPI_Request Req[2];
	MPI_Status Stat[2];
	int* arrSend = new int[arrSize];
	int* arrRecv = new int[arrSize];

	for (int i = 0; i < arrSize; ++i) {
		arrSend[i] = rank;
	}
	MPI_Isend(arrSend, arrSize, MPI_INT, (rank + 1) % total, 1, MPI_COMM_WORLD, &Req[0]);
	MPI_Wait(&Req[0], &Stat[0]);

	MPI_Irecv(arrRecv, arrSize, MPI_INT, (total + rank - 1) % total, 1, MPI_COMM_WORLD, &Req[0]);
	MPI_Wait(&Req[0], &Stat[0]);

	std::cout << "Rank " << rank << " generated:" << std::endl;
	for (int i = 0; i < arrSize; ++i)
		std::cout << arrSend[i] << " ";

	std::cout << std::endl;

	std::cout << "Rank: " << rank << " received:" << std::endl;
	for (int i = 0; i < arrSize; i++)
		std::cout << arrRecv[i] << " ";
	std::cout << std::endl;

	MPI_Finalize();
}