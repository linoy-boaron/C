/**
 * @file CompareSequences.c
 * @author Linoy Boaron <linoy.boaron@mail.huji.ac.il>
 * @version 1.0
 * @date 14 Nov', 2018
 *
 * @brief  program to compare between sequences.
 *
 * @section DESCRIPTION
 * The program get a file with list of sequences
 * and compare them by matrix Used the weights that given,and return the final score of the best
 *match of them.
 *
 * Input  : text file,  weights of match mis match and gap .
 * Process:  compare sequences and get the final score of best match between them.
 * Output : the final score of best match between sequences.
 */

// ------------------------------ Includes ------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

// -------------------------- const definitions -------------------------

#define FILE_PERM "r"
#define NULL_FILE "Error opening file: %s\n"
#define PROGRAM_USAGE_MESSAGE "Usage: CompareSequences <path_to_sequences_file> <m> <s> <g>\n"
#define NUM_OF_ARGC 5
#define MEMORY_ERROR "Error problem of the requested memory.\n"
#define EMPTY_ERROR "Error The given file is empty.\n"
#define ERROR_ONLY_ONE "Error There is only one seq.\n "
#define ERROR_INT_WEIGHTS "Error in weights.\n"
#define SCORE_MSG "Score for alignment of %s to %s is %d\n"
#define MIN_SEQUENCES 2
#define PATH 1
#define LINE_LEN 101
#define MATCH 2
#define MISMATCH 3
#define GAP 4
#define HEADER_PREFIX ">"
#define HEADER_PREFIX_LOC 0


#define ASSERT_ALLOC(var) \
    if (var == NULL) \
    { \
        fprintf(stderr, MEMORY_ERROR); \
        exit(EXIT_FAILURE); \
    }
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

/**
 * struct represents weights of match, mismatch and gap
 */
typedef struct
{
    int match;
    int mismatch;
    int gap;
} Weights;

/**
 * struct Represents each paragraph that includes the header;seq;len
 */
typedef struct
{
    char *header;
    char *sequence;
    size_t length;
} Sequence;

/**
 * //struct Represents Sequences include the sequences and his size
 */

typedef struct
{
    Sequence **sequences;
    size_t size;
} Sequences;

// ------------------------------ functions -----------------------------

//Receives a path to text, checks for propriety, identifies whether it is a header or a
// sentence and stores in a struct accordingly.
Sequences *readSequences(char *filePath);
// change to a int type
int toInt(char *val);
//Compares Sequences  by constructing an appropriate matrix and calculating its
// final score using auxiliary functions
void compareSequences(Sequences *sequences, Weights weights);
// Releasing the memory that we gave to the structs
void freeAll(Sequences *sequences);
//remove New Line Char
void removeNewLineChar(char *line);
//Checks whether this is a header or a comparison sentence
int isHeader(const char *line);
//add Sequence to Sequences struct
void addSequence(Sequences *sequences, Sequence *seq);
//add seq to sequence struct and in fact connect between seq
void concatSequence(Sequence *seq, const char *str);
//Releasing the memory that we gave to the Sequence
void freeSequence(Sequence *sequence);
//Initializes a matrix according to the sentences and weights
int **initTable(char *seq1, char *seq2, Weights weights);
//calculate the final score of comparing 2 seq
int calculateTable(char *seq1, char *seq2, Weights weights);
//check num of seq in the given text
void checkSeqNum(Sequences *sequences);
//check if the given file is empty
void checkIfEmptyFile(FILE *file);
//allocate memory to new Sequence struct
Sequence *allocNewSequence();
//initializes new sequences struct
Sequences *initSequences();
//Releasing the memory that we gave to the matrix
void freeTable(int **dynamicTable, int rows);
//get the header in the text
char* getHeader(const char* line);


//get the header in the text

char* getHeader(const char* line)
{
    size_t prefixLen = strlen(HEADER_PREFIX);
    size_t len = strlen(line) - (HEADER_PREFIX_LOC + strlen(HEADER_PREFIX));
    char* result = malloc(sizeof(char) * (len + 1));
    ASSERT_ALLOC(result);
    memset(result, '\0', len + 1);
    strncpy(result, line + HEADER_PREFIX_LOC + prefixLen, len);
    return result;
}

//Receives a path to text, checks for propriety, identifies whether it is a header or a
// sentence and stores in a struct accordingly.
Sequences *readSequences(char *filePath)
{
    FILE *file = fopen(filePath, FILE_PERM);

    if (file == NULL)
    {
        fprintf(stderr, NULL_FILE, filePath);
        exit(EXIT_FAILURE);
    }

    checkIfEmptyFile(file);
    char line[LINE_LEN] = {};
    memset(line, '\0', LINE_LEN);

    Sequences *sequences = initSequences();
    ASSERT_ALLOC(sequences);

    Sequence *cur = NULL;
    while (fgets(line, LINE_LEN, file))
    {
        size_t len = strlen(line);
        if (line[0] != '\n' && len > 0)
        {
            removeNewLineChar(line);

            if (isHeader(line))
            {
                if (cur != NULL)
                {
                    addSequence(sequences, cur);
                }

                cur = allocNewSequence();
                cur->header = getHeader(line);
            }
            else
            {
                concatSequence(cur, line);
            }
        }
    }

    addSequence(sequences, cur);

    fclose(file);
    checkSeqNum(sequences);
    return sequences;
}
//initializes new sequences struct

Sequences *initSequences()
{
    Sequences *sequences = malloc(sizeof(Sequences));
    ASSERT_ALLOC(sequences);
    sequences->sequences = malloc(sizeof(Sequence *));
    ASSERT_ALLOC(sequences);
    sequences->size = 0;
    return sequences;

}
//Checks whether this is a header or a comparison sentence

int isHeader(const char *line)
{
    return line[0] == '>';
}
//allocate memory to new Sequence struct

Sequence *allocNewSequence()
{
    Sequence *seq = malloc(sizeof(Sequence));
    ASSERT_ALLOC(seq);
    seq->sequence = malloc(sizeof(char) * 1);
    seq->sequence[0] = '\0';
    seq->length = 0;
    return seq;
}
//add Sequence to Sequences struct
void addSequence(Sequences *sequences, Sequence *seq)
{
    sequences->sequences = realloc(sequences->sequences,
                                   sizeof(Sequence *) * (sequences->size + 1));
    ASSERT_ALLOC(sequences);
    sequences->sequences[sequences->size] = seq;
    sequences->size++;

}

//add seq to sequence struct and in fact connect between seq
void concatSequence(Sequence *seq, const char *str)
{
    size_t len = sizeof(char) * (strlen(seq->sequence) + strlen(str) + 1);
    seq->sequence = realloc((void *)seq->sequence, len);
    seq->sequence = strcat(seq->sequence, str);
    seq->sequence[len - 1] = '\0';
}

//remove New Line Char

void removeNewLineChar(char *line)
{
    size_t len = strlen(line);
    if (line[len - 1] == '\n')
    {
        line[len - 1] = '\0';
    }

    if (line[len - 2] == '\r')
    {
        line[len - 2] = '\0';
    }
}

// change to a int type

int toInt(char *val)
{
    long result = 0;
    char *end;
    errno = 0;
    result = strtol(val, &end, 10);
    if ((errno != 0 || end == val) && result == 0)
    {
        fprintf(stderr, ERROR_INT_WEIGHTS);
        exit(EXIT_FAILURE);
    }

    return (int) result;
}
// Releasing the memory that we gave to the structs

void freeAll(Sequences *sequences)
{
    for (size_t i = 0; i < sequences->size; i++)
    {
        freeSequence(sequences->sequences[i]);
        sequences->sequences[i] = NULL;
    }
    free(sequences->sequences);
    free(sequences);
}

//Releasing the memory that we gave to the Sequence

void freeSequence(Sequence *sequence)
{
    if (sequence != NULL)
    {
        free(sequence->header);
        free(sequence->sequence);
        free(sequence);
    }
}

//Compares Sequences  by constructing an appropriate matrix and calculating its
// final score using auxiliary functions
void compareSequences(Sequences *sequences, Weights weights)
{
    size_t len = sequences->size;
    for (size_t i = 0; i < len; i++)
    {
        for (size_t j = i + 1; j < len; j++)
        {
            char *seqA = sequences->sequences[i]->sequence;
            char *seqB = sequences->sequences[j]->sequence;
            int score = calculateTable(seqA, seqB, weights);
            printf(SCORE_MSG, sequences->sequences[i]->header, sequences->sequences[j]->header, score);
        }
    }
}
//Initializes a matrix according to the sentences and weights


int **initTable(char *seq1, char *seq2, Weights weights)
{
    int cols = (int)strlen(seq1) + 1;
    int rows = (int)strlen(seq2) + 1;
    int **table = malloc((rows) * sizeof(int *));

    for (int i = 0; i < rows; i++)
    {
        table[i] = (int *) malloc(cols * sizeof(int));
    }

    for (int j = 0; j < cols; j++)
    {
        table[0][j] = weights.gap * j;
    }

    for (int s = 0; s < rows; s++)
    {
        table[s][0] = weights.gap * s;
    }

    return table;
}
//Releasing the memory that we gave to the matrix

void freeTable(int **dynamicTable, int rows)
{
    for (int i = 0; i < rows; i++)
    {
        free(dynamicTable[i]);
    }
    free(dynamicTable);
}

//External function for calculating the final score of a comparison between two sentences by a
// matrix

int calculateScore(int** table, char* seq1, char* seq2, size_t i, size_t j, Weights weights)
{
    int diagonal = table[i - 1][j - 1];
    if (seq1[j - 1] == seq2[i - 1])
    {
        diagonal += weights.match;
    }
    else
    {
        diagonal += weights.mismatch;
    }

    int left = table[i][j - 1] + weights.gap;
    int bottom = table[i - 1][j] + weights.gap;

    return MAX(diagonal, MAX(left, bottom));
}
//calculate the final score of comparing 2 seq

int calculateTable(char *seq1, char *seq2, Weights weights)
{
    int **dynamicTable = initTable(seq1, seq2, weights);
    size_t rows = strlen(seq2) + 1;
    size_t cols = strlen(seq1) + 1;

    for (size_t i = 1; i < rows; i++)
    {
        for (size_t j = 1; j < cols; j++)
        {
            dynamicTable[i][j] = calculateScore(dynamicTable, seq1, seq2, i, j, weights);
        }

    }

    int score = dynamicTable[rows - 1][cols - 1];
    freeTable(dynamicTable, (int)strlen(seq2) + 1);
    return score;
}
//check num of seq in the given text

void checkSeqNum(Sequences *sequences)
{
    if (sequences->size < MIN_SEQUENCES)
    {
        fprintf(stderr, ERROR_ONLY_ONE);
        freeAll(sequences);
        exit(EXIT_FAILURE);
    }
}
//check if the given file is empty

void checkIfEmptyFile(FILE *file)
{
    fseek(file, 0, SEEK_END);
    if (ftell(file) < 1)
    {
        fprintf(stderr, EMPTY_ERROR);
        fclose(file);
        exit(EXIT_FAILURE);
    }

    rewind(file);
}
// start the program .  receive argc and Accordingly, doing checks on the given file, if empty,
// if contains a   valid number of sentences  then compares the sequences and brings the maximum
// final score of best match between them.
int main(int argc, char *argv[])
{
    if (argc != NUM_OF_ARGC)
    {
        fprintf(stderr, PROGRAM_USAGE_MESSAGE);
        exit(EXIT_FAILURE);
    }

    Sequences *sequences = readSequences(argv[PATH]);
    Weights weights = {toInt(argv[MATCH]), toInt(argv[MISMATCH]), toInt(argv[GAP])};
    compareSequences(sequences, weights);
    freeAll(sequences);
    return EXIT_SUCCESS;
}

