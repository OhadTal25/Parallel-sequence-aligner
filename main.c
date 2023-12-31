#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#include <string.h>    // Add this line to include string.h for strcpy function
#include "functions.h" // Add this line to include your functions header

#define MAX_SEQ_LENGTH 3000
#define MAX_NUM_SEQUENCES 2000
#define MASTER 0

// Declare the global sequences array
extern char **sequences;

// Define the scoreTable
extern int scoreTable[26][26]; // 26x26 matrix

int main(int argc, char *argv[])
{
    sequences = NULL;
    char mainSequence[MAX_SEQ_LENGTH] = {0};
    int numSequences;
    char scoreTableFilename[256]; // Assuming a reasonable filename length
    int *localResults;
    int *globalResults = NULL;

    // Check if a command-line argument for the score table file is provided
    if (argc >= 2)
    {
        strcpy(scoreTableFilename, argv[1]);
        // Read the score table from the file and initialize scoreTable array
        readScoreTableFromFile(scoreTableFilename);
    }
    else
    {
        // initialize scoreTable array with default values
        readScoreTableFromFile(NULL);
    }
    // A time for calculation
    double startTime, endTime;

    MPI_Init(&argc, &argv);
    int rank, numProcesses;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);

    //  printf("This is the num of proseces: %d\n", numProcesses);
    // Record start time
    startTime = MPI_Wtime();

    // Read input only in the root process
    if (rank == MASTER)
    {

        int scannedItems = scanf("%s\n", mainSequence); // The first sequence
        if (scannedItems != 1)
        {
            fprintf(stderr, "Error reading mainSequence.\n");
            MPI_Abort(MPI_COMM_WORLD, 1); // Abort MPI processes
        }
        strToUpper(mainSequence); // converting mainSequence to uppercase

        scannedItems = scanf("%d\n", &numSequences); // The number of sequences
        if (scannedItems != 1)
        {
            fprintf(stderr, "Error reading numSequences.\n");
            MPI_Abort(MPI_COMM_WORLD, 1); // Abort MPI processes
        }
    }

    // Broadcast mainSequence and numSequences from root to other processes
    MPI_Bcast(mainSequence, MAX_SEQ_LENGTH, MPI_CHAR, MASTER, MPI_COMM_WORLD);
    MPI_Bcast(&numSequences, 1, MPI_INT, MASTER, MPI_COMM_WORLD);

    int chunkSize = numSequences / numProcesses; // How much work each process needs to do

    if (rank == MASTER)
    {
        sequences = (char **)malloc(numSequences * (sizeof(char *)));
        for (int i = 0; i < numSequences; i++)
        {
            // allocates memory block for the current sequence
            sequences[i] = (char *)calloc(MAX_SEQ_LENGTH, sizeof(char));
            if (sequences[i] == NULL)
            {
                fprintf(stderr, "Error allocating sequence %d.\n", i);
                MPI_Abort(MPI_COMM_WORLD, 1); // Abort MPI processes
            }

            int scannedItems = scanf("%s", sequences[i]);
            if (scannedItems != 1)
            {
                fprintf(stderr, "Error reading sequence %d.\n", i);
                MPI_Abort(MPI_COMM_WORLD, 1); // Abort MPI processes
            }
            strToUpper(sequences[i]); // converting sequences[i] to uppercase
        }
    }

    // Allocate memory for the local sequences that each process will receive
    char **localSequences = (char **)malloc(chunkSize * sizeof(char *));
    for (int i = 0; i < chunkSize; i++)
    {
        localSequences[i] = (char *)calloc(MAX_SEQ_LENGTH, sizeof(char));
        if (localSequences[i] == NULL)
        {
            fprintf(stderr, "Error allocating L. sequence %d.\n", i);
            MPI_Abort(MPI_COMM_WORLD, 1); // Abort MPI processes
        }
    }

    /* Send data from the main sequences array to the local sequences array of each process
       send 'chunkSize' number of sequences, each with 'MAX_SEQ_LENGTH' characters*/
    if (rank == MASTER)
    {
        for (int curr = 0; curr < chunkSize; curr++)
        { // copy first chunkSize sequences to localSequences
            strcpy(localSequences[curr], sequences[curr]);
        }
        for (int sendTo = 1; sendTo < numProcesses; sendTo++)
        {                                       // loop though all other processes
            for (int i = 0; i < chunkSize; i++) // sending chunkSize sequences to slaves
                MPI_Send(sequences[sendTo * chunkSize + i], MAX_SEQ_LENGTH, MPI_CHAR, sendTo, 0, MPI_COMM_WORLD);
        }
    }
    else // slave
    {
        for (int i = 0; i < chunkSize; i++) // reciving chunkSize sequences from master
            MPI_Recv(localSequences[i], MAX_SEQ_LENGTH, MPI_CHAR, MASTER, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    localResults = (int *)malloc(chunkSize * sizeof(int) * 3); // alignmentScore, alignment ,k for each seq
    for (int i = 0; i < chunkSize; i++)
    {
        // Process localSequences and calculate alignment scores
        // Call the calcAlignmentScore function and calculate alignmentScore, alignment ,k (mutant)
        calcAlignmentScore(localSequences[i], mainSequence, &localResults[3 * i], &localResults[3 * i + 1], &localResults[3 * i + 2]);
    }

    if (rank == MASTER)
    {
        globalResults = (int *)malloc(sizeof(int) * numSequences * 3);
    }
    // gather all results from all process 
    MPI_Gather(localResults, chunkSize * 3, MPI_INT, globalResults, chunkSize * 3, MPI_INT, MASTER, MPI_COMM_WORLD);

    // Record end time
    endTime = MPI_Wtime();
    // Calculate total execution time
    double totalTime = endTime - startTime;
    if (rank == MASTER)
    {
        // Print results
        printResults(globalResults, numSequences);
        // Print timing information
        printf("Total execution time: %f seconds\n", totalTime);
    }

    for (int i = 0; i < chunkSize; i++)
        free(localSequences[i]);
    free(localSequences); // Clean up, free the memory allocated for the local sequences array
    free(localResults);

    if (rank == 0)
    {
        free(globalResults);
        // Free the memory allocated for each sequence in the main sequences array
        for (int i = 0; i < numSequences; i++)
        {
            free(sequences[i]);
        }
        free(sequences); // Free the memory allocated for the main sequences array
    }
    MPI_Finalize(); // Finalize the MPI environment
    return 0;       // Return 0 to indicate successful completion of the program
}
