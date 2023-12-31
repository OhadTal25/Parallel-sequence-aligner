#include "functions.h"
#define MAX_SEQ_LENGTH 3000

// Declare the global sequences array
char **sequences;

// Define the scoreTable
int scoreTable[26][26]; // 26x26 matrix

void readScoreTableFromFile(const char *filename)
{
    FILE *file = NULL;
    if (filename)
        file = fopen(filename, "r");
    if (file == NULL)
    {
        // If file not found or invalid, fall back to default
        for (int i = 0; i < 26; i++)
        {
            for (int j = 0; j < 26; j++)
            {
                if (i == j)
                {
                    scoreTable[i][j] = 1;
                }
                else
                {
                    scoreTable[i][j] = 0;
                }
            }
        }
        return;
    }

    // Read scores from the file and update the scoreTable array
    for (int i = 0; i < 26; i++)
    {
        for (int j = 0; j < 26; j++)
        {
            if (fscanf(file, "%d", &scoreTable[i][j]) != 1)
            {
                printf("Error reading score from file. Using default score table.\n");
                fclose(file);
                // If there's an error reading scores, fall back to default
                for (int i = 0; i < 26; i++)
                {
                    for (int j = 0; j < 26; j++)
                    {
                        if (i == j)
                        {
                            scoreTable[i][j] = 1;
                        }
                        else
                        {
                            scoreTable[i][j] = 0;
                        }
                    }
                }
                return;
            }
        }
    }
    if (file)
        fclose(file);
}

void calcAlignmentScore(char *localSequence, char *mainSequence, int *alignmentScore, int *alignment, int *mutation)
{
    int mainSSize = (int)strlen(mainSequence);
    int localSSize = (int)strlen(localSequence);
    /*setting default values*/
    *alignmentScore = INT_MIN;
    *alignment = -1;
    *mutation = -1;

    int currentScore = 0;
    for (int offset = 0; offset <= mainSSize - localSSize; offset++) // loop over all avaliable offsets
    {
        for (int k = 0; k <= localSSize; k++) // loop over all avaliable mutations
        {
            currentScore = 0;
            for (int index = 0; index < localSSize; index++) // loop over all the chars in localSequence
            {
                int isMutation = index >= k ? 1 : 0;
                currentScore += calcScoreOfTwoChars(localSequence[index] + isMutation, mainSequence[index + offset]);
            }
            if (currentScore > (*alignmentScore))
            {
                *alignmentScore = currentScore;
                *alignment = offset;
                *mutation = k;
            }
        }
    }
}

int calcScoreOfTwoChars(char c1, char c2)
{
    int pos1 = c1 - 'A';
    if (c1 == 'Z' + 1)
        pos1 = 0;
    return scoreTable[pos1][c2 - 'A'];
}

void strToUpper(char *str)
{ // converting str to uppercase
    for (int i = 0; str[i]; i++)
        str[i] = toupper(str[i]);
}

void printResults(int *results, int numSequences)
{
    for (int i = 0; i < numSequences; i++)
    {
        printf("Hightest alignmentScore = %d  offset = %d  k = %d \n", results[3 * i], results[3 * i + 1], results[3 * i + 2]);
    }
}
