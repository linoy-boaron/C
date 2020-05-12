/**
 * @file AnalyzeProtein.c
 * @author  Linoy Boaron <linoy.boaron@mail.huji.ac.il>
 * @version 1.0
 * @date 1  Nov 2018
 *
 * @brief program that analyze proteins.
 *
 * @section DESCRIPTION
 * program that analyze the protein files.
 * Input  : Protein Files (pdb)
 * Process: analyzing the proteins.
 * Output : .the protein's gravity center,atoms radius and the maximum
 * distance between atoms.
 */

// ------------------------------ includes ------------------------------
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
// -------------------------- const definitions -------------------------
#define LEN_OF_LINE  81
#define MAX_NUM_OF_ATOMS  20000
#define CORDS_NUM 3
#define FIRST_ARG 1
#define ATOM_LEN 6
#define ATOM 7
#define START_WITH_ATOM "ATOM  "
#define SHORT_LINE_LEN 60
#define X_CORD 0
#define Y_CORD 1
#define Z_CORD 2
#define X_CORD_IN_LINE 30
#define Y_CORD_IN_LINE 38
#define Z_CORD_IN_LINE 46
#define CORD_LEN 8
#define CHAR_CORD_LEN 9
#define FILE_MSG "PDB file %s, %d atoms were read\nCg = %.3f %.3f %.3f\nRg = %.3f\nDmax = %.3f\n"
#define ARGC_NUM_ERROR "Usage: AnalyzeProtein <pdb1> <pdb2> ..."
#define NULL_FILE "Error opening file: %s\n"
#define CONVERSION_ERROR "Error in coordinate conversion %s!\n"
#define EMPTY_FILE_ERROR "Error: file is empty\n"
#define NO_ATOMS_ERROR "Error - 0 atoms were found in the file %s\n"
#define SHORT_LINE "ATOM line is too short %zu characters\n"
/**
 *read the file and make sure there are atoms on the file
 * @param coordinates
 * @param file
 * @param fileName
 * @return num of atoms in the file .
 *
 */
int readAtoms(float coordinates[MAX_NUM_OF_ATOMS][CORDS_NUM], FILE *file, char fileName[]);

/**
 * extract Coordinates of the atoms from their row
 * @param arr
 * @param cords
 */
void extractCoordinate(char arr[], float *cords);

/**
 * calculate the Gravity Center of the protein
 * @param coordinates
 * @param gc
 * @param numOfAtoms
 *
 */

void getGravityCenter(float coordinates[MAX_NUM_OF_ATOMS][CORDS_NUM], float gc[CORDS_NUM],
                      int numOfAtoms);
/**
 * calculate the radius of the protein
 * @param center
 * @param numOfAtoms
 * @param cg
 * @return  radius of the protein
 */
float radius(float center[MAX_NUM_OF_ATOMS][CORDS_NUM], int numOfAtoms, float cg[CORDS_NUM]);
/**
 * calculate the distance  between atoms
 * @param a
 * @param b
 * @return distance between 2 atoms
 */
float distance(float a[CORDS_NUM], float b[CORDS_NUM]);
/**
 * extract single coord of the atom from the row
 * @param singleCord
 * @param arr
 * @param index
 */
void extractSingleCoord(char singleCord[9], const char *arr, int index);

/**
 * print results of the analyzing of the file
 * @param fileName
 * @param numOfAtoms
 * @param gc
 * @param atoms
 *
 */
void
printFile(char *fileName, int numOfAtoms, float gc[3], float atoms[MAX_NUM_OF_ATOMS][CORDS_NUM]);

/**
 * calculate the maximum distance between atoms
 * @param cord
 * @param numOfAtoms
 * @return maximum distance between atoms.
 */
float dMax(float cord[MAX_NUM_OF_ATOMS][CORDS_NUM], int numOfAtoms);

/**
 * check num of args
 * @param argc
 */
void checkArgcNum(int argc);
/**
 * check if the file is proper
 * @param file
 * @param fileName
 */
void checkNull(FILE *file, const char fileName[]);
/**
 * check error of Conversion in the file
 * @param cord
 * @param input
 * @param end
 * @param val
 */
void checkConversion(float cord, const char input[], const char *end, const char val[]);
/**
 * check error of no atoms/lines in the file
 * @param countLines
 * @param countAtom
 * @param fileName
 */
void checkFile(int countLines, int countAtom, char fileName[]);
/**
 * check error of line too long
 * @param line
 */
void checkLineLenGreaterThan60(const char *line);

/**
 * runs the whole program calls the whole function and causes the program to work
 * @param argc
 * @param argv
 * @return 0 if the running was ok :)
 */
int main(int argc, char *argv[])
{
    checkArgcNum(argc);
    for (int i = FIRST_ARG; i < argc; i++)
    {
        FILE *file = fopen(argv[i], "r");
        checkNull(file, argv[i]);
        float atoms[MAX_NUM_OF_ATOMS][CORDS_NUM];
        int numOfAtoms = readAtoms(atoms, file, argv[i]);
        float gravityCenter[CORDS_NUM] = {0.0f, 0.0f, 0.0f};
        getGravityCenter(atoms, gravityCenter, numOfAtoms);
        printFile(argv[i], numOfAtoms, gravityCenter, atoms);
        fclose(file);
    }
}

/**
 *
 * @param coordinates
 * @param file
 * @param fileName
 * @return num of atoms in the file .
 * read the file and make sure there are atoms on the file
 */
int readAtoms(float coordinates[MAX_NUM_OF_ATOMS][CORDS_NUM], FILE *file, char fileName[])
{
    int countAtoms = 0;
    int countLines = 0;
    char lineStart[ATOM];
    char line[LEN_OF_LINE];

    while (fgets(line, LEN_OF_LINE, file) != NULL && countAtoms < MAX_NUM_OF_ATOMS)
    {
        countLines++;
        strncpy(lineStart, line, ATOM_LEN);
        lineStart[ATOM_LEN] = '\0';
        if (strcmp(lineStart, START_WITH_ATOM) == 0)
        {
            checkLineLenGreaterThan60(line);
            extractCoordinate(line, coordinates[countAtoms]);
            countAtoms++;
        }

    }
    checkFile(countLines, countAtoms, fileName);
    return countAtoms;
}

/**
 *
 * @param arr
 * @param cords
 * extract Coordinates of the atoms from their row
 */
void extractCoordinate(char arr[], float cords[CORDS_NUM])
{
    char x[CHAR_CORD_LEN] = "", y[CHAR_CORD_LEN] = "", z[CHAR_CORD_LEN] = "";
    extractSingleCoord(x, arr, X_CORD_IN_LINE);
    extractSingleCoord(y, arr, Y_CORD_IN_LINE);
    extractSingleCoord(z, arr, Z_CORD_IN_LINE);
    errno = 0;
    char *end;
    cords[X_CORD] = strtof(x, &end);
    checkConversion(cords[X_CORD], x, end, x);
    cords[Y_CORD] = strtof(y, &end);
    checkConversion(cords[Y_CORD], y, end, y);
    cords[Z_CORD] = strtof(z, &end);
    checkConversion(cords[Z_CORD], z, end, z);
}

/**
 *
 * @param singleCord
 * @param arr
 * @param index
 * extract single coord of the atom from the row
 */

void extractSingleCoord(char singleCord[CHAR_CORD_LEN], const char *arr, int index)
{
    for (int i = 0; i < CORD_LEN; i++)
    {
        singleCord[i] = arr[i + index];
    }
}

/**
 * calculate the Gravity Center of the protein
 * @param coordinates
 * @param gc
 * @param numOfAtoms
 *
 *
 */
void getGravityCenter(float coordinates[MAX_NUM_OF_ATOMS][CORDS_NUM], float gc[CORDS_NUM],
                      int numOfAtoms)
{
    for (int i = 0; i < numOfAtoms; i++)
    {
        gc[X_CORD] += coordinates[i][X_CORD];
        gc[Y_CORD] += coordinates[i][Y_CORD];
        gc[Z_CORD] += coordinates[i][Z_CORD];
    }
    gc[X_CORD] /= numOfAtoms;
    gc[Y_CORD] /= numOfAtoms;
    gc[Z_CORD] /= numOfAtoms;
}

/**
 * calculate the radius of the protein
 * @param center
 * @param numOfAtoms
 * @param cg
 * @return  radius of the protein
 */
float radius(float center[MAX_NUM_OF_ATOMS][CORDS_NUM], int numOfAtoms, float cg[CORDS_NUM])
{
    float sumDistance = 0;
    for (int i = 0; i < numOfAtoms; i++)
    {
        sumDistance += pow(distance(cg, center[i]), 2);
    }
    return (float) sqrt((double) sumDistance / numOfAtoms);


}

/**
 * calculate the maximum distance between atoms
 * @param cord
 * @param numOfAtoms
 * @return maximum distance between atoms.
 */
float dMax(float cord[MAX_NUM_OF_ATOMS][CORDS_NUM], int numOfAtoms)
{
    float maxDistance = 0;
    for (int i = 0; i < numOfAtoms - 1; i++)
    {
        for (int j = i + 1; j < numOfAtoms; j++)
        {
            if (distance(cord[i], cord[j]) > maxDistance)
            {
                maxDistance = distance(cord[i], cord[j]);
            }
        }
    }
    return maxDistance;
}

/**
 * calculate the distance  between atoms
 * @param a
 * @param b
 * @return distance between 2 atoms
 */
float distance(float a[CORDS_NUM], float b[CORDS_NUM])
{

    return sqrtf(powf((a[0] - b[0]), 2) + powf((a[1] - b[1]), 2) + powf((a[2] - b[2]), 2));

}

/**
 * print results of the analyzing of the file
 * @param fileName
 * @param numOfAtoms
 * @param gc
 * @param atoms
 *
 */
void
printFile(char *fileName, int numOfAtoms, float gc[3], float atoms[MAX_NUM_OF_ATOMS][CORDS_NUM])
{
    printf(FILE_MSG, fileName, numOfAtoms, gc[0], gc[1], gc[2], radius(atoms, numOfAtoms, gc),
           dMax(atoms, numOfAtoms));
}

/**
 * check num of args
 * @param argc
 */
void checkArgcNum(int argc)
{
    if (argc < 2)
    {
        fprintf(stdout, ARGC_NUM_ERROR);
        exit(EXIT_FAILURE);
    }
}

/**
 * check if the file is proper
 * @param file
 * @param fileName
 */
void checkNull(FILE *file, const char fileName[])
{
    if (file == NULL)
    {
        fprintf(stderr, NULL_FILE, fileName);
        exit(EXIT_FAILURE);
    }
}

/**
 * check error of Conversion in the file
 * @param cord
 * @param input
 * @param end
 * @param val
 */
void checkConversion(float cord, const char input[], const char *end, const char val[])
{
    if (cord == 0 && (errno != 0 || input == end))
    {
        fprintf(stderr, CONVERSION_ERROR, val);
        exit(EXIT_FAILURE);
    }
    errno = 0;
}

/**
 * check error of no atoms/lines in the file
 * @param countLines
 * @param countAtom
 * @param fileName
 */
void checkFile(int countLines, int countAtom, char fileName[])
{
    if (countLines == 0)
    {
        fprintf(stderr, EMPTY_FILE_ERROR);
        exit(EXIT_FAILURE);
    }
    if (countAtom == 0)
    {
        fprintf(stderr, NO_ATOMS_ERROR, fileName);
        exit(EXIT_FAILURE);
    }
}

/**
 * check error of line too long
 * @param line
 */
void checkLineLenGreaterThan60(const char *line)
{
    size_t len = strlen(line);
    if (len <= SHORT_LINE_LEN)
    {
        fprintf(stderr, SHORT_LINE, len);
        exit(EXIT_FAILURE);
    }
}