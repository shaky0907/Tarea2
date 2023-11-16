#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

// Temporary array for slave process
int a2[1000];

clock_t start_time,end_time;

int writeResults(int msg_size,double time){

 	// Open a file for writing (create if it doesn't exist, truncate to zero length otherwise)
    FILE *file = fopen("data.txt", "a");

    // Check if the file was opened successfully
    if (file == NULL) {
        fprintf(stderr, "Error opening file.\n");
        return 1; // Return an error code
    }

    fprintf(file, "%d;%f\n", msg_size, time);

    // Close the file when you're done
    fclose(file);

	return 0;

}






void start(int argc, char* argv[]){
	int pid, np,
		n_elements_recieved;
	// np -> no. of processes
	// pid -> process id
	int message_size;

	//size of array
	int n = 1000;

	MPI_Status status;

	// Creation of parallel processes
	//MPI_Init(&argc, &argv);

	// find out process ID,
	// and how many processes were started
	MPI_Comm_rank(MPI_COMM_WORLD, &pid);
	MPI_Comm_size(MPI_COMM_WORLD, &np);

	// master process
	if (pid == 0) {
		start_time = clock();
		
        printf("Enter message size: ");
        scanf("%d", &message_size);

        int a[n];

        for (int i = 0; i < n; ++i) {
            a[i] = i+1;
        }

		int b[np];
		int j = 0 ;

		//inicializar array
		for (int i = 0; i < np; i += 1) {
            b[i] = 0;
        }

		while (j < n){
			for (int i = 0; i < np; i += 1) {
				if(j + message_size > n){
					b[i] += n-j;
				}
				else{
					b[i] += message_size;
				}
			
				j += message_size;
				if (j >= n){
					break;
				}
			}
        }
		
		
	
		int index, i;
		
		
		// check if more than 1 processes are run
		if (np > 1) {
			// distributes the portion of array
			// to child processes to calculate
			// their partial sums
			int index = b[0];
			for (i = 1; i < np - 1; i++) {
				
				MPI_Send(&b[i],
						1, MPI_INT, i, 0,
						MPI_COMM_WORLD);
				MPI_Send(&a[index],
						b[i],
						MPI_INT, i, 0,
						MPI_COMM_WORLD);
				index += b[i];
			}

			// last process adds remaining elements
			int elements_left = n - index;
			
			MPI_Send(&b[i],
					1, MPI_INT,
					i, 0,
					MPI_COMM_WORLD);
			MPI_Send(&a[index],
					b[i],
					MPI_INT, i, 0,
					MPI_COMM_WORLD);
		}

		// master process add its own sub array
		int sum = 0;
		for (i = 0; i < b[0]; i++)
			sum += a[i];

        printf("sum calculated by root process: %d\n", sum);


		// collects partial sums from other processes
		int tmp;
		for (i = 1; i < np; i++) {
			MPI_Recv(&tmp, 1, MPI_INT,
					MPI_ANY_SOURCE, 0,
					MPI_COMM_WORLD,
					&status);
			int sender = status.MPI_SOURCE;

            printf("Sum calculated by process %d: %d\n", sender, tmp);

			sum += tmp;

		}

		// prints the final sum of array
		printf("Sum of array is : %d\n", sum);


		//tiempo de finalizacion
		end_time = clock();
		double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
		printf("Execution Time: %.6f seconds\n", elapsed_time);
		writeResults(message_size,elapsed_time);
	}

	// slave processes
	else {
		MPI_Recv(&n_elements_recieved,
				1, MPI_INT, 0, 0,
				MPI_COMM_WORLD,
				&status);

		// stores the received array segment
		// in local array a2
		MPI_Recv(&a2, n_elements_recieved,
				MPI_INT, 0, 0,
				MPI_COMM_WORLD,
				&status);

		// calculates its partial sum
		int partial_sum = 0;
		for (int i = 0; i < n_elements_recieved; i++)
			partial_sum += a2[i];

        printf("Partial sum returned from process %d: %d\n", pid, partial_sum);

		// sends the partial sum to the root process
		MPI_Send(&partial_sum, 1, MPI_INT,
				0, 0, MPI_COMM_WORLD);
	}

	// cleans up all MPI state before exit of process
	MPI_Finalize();

}


int main(int argc, char* argv[])
{	
	int pid;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &pid);


	//ejecucion
	start(argc,argv);


	return 0;
}

