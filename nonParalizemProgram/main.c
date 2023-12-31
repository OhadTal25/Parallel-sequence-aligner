#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#include <string.h>    
#include "functions.h" 

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

    double startTime, endTime;

    // Record start time
    MPI_Init(&argc, &argv);
    startTime = MPI_Wtime();

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

    localResults = (int *)malloc(numSequences * sizeof(int) * 3); // alignmentScore, alignment ,k for each seq

    for (int i = 0; i < numSequences; i++)
    {
        // Process localSequences and calculate alignment scores
        // Call the calcAlignmentScore function and calculate alignmentScore, alignment ,k (mutant)
        calcAlignmentScore(sequences[i], mainSequence, &localResults[3 * i], &localResults[3 * i + 1], &localResults[3 * i + 2]);
    }

    // Record end time
    endTime = MPI_Wtime();
    // Calculate total execution time
    double totalTime = endTime - startTime;
    // Print results
    printResults(localResults, numSequences);
    // Print timing information
    printf("Total execution time: %f seconds\n", totalTime);

    free(localResults);

    // Free the memory allocated for each sequence in the main sequences array
    for (int i = 0; i < numSequences; i++)
    {
        free(sequences[i]);
    }
    free(sequences); // Free the memory allocated for the main sequences array
    MPI_Finalize(); // Finalize the MPI environment
    return 0;        // Return 0 to indicate successful completion of the program
}
