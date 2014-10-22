#ifndef DEALWITHFILE_H
#define DEALWITHFILE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void removeComents(char *file_contents,int *size);
//char *getNextWord(FILE **arq);
char **fileToMatriz(FILE *arq, int **num_lines, int **num_chars_per_line);
char *fileToVector(FILE *source, int **size_contents);
void compileProgram(char *file_contents,int *size);
#endif // DEALWITHFILE_H
