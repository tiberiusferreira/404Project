#ifndef DEALWITHFILE_H
#define DEALWITHFILE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void removeComents(char *file_contents,int *size);
char **fileToMatriz(FILE *arq, int **num_lines, int **num_chars_per_line);
char *fileToVector(FILE *source, int **size_contents);
void getNextWord(char *current_word, int *current_source_line,int *current_source_line_word, int *size_current_word, int *i, char *file_contents, int size_file_contents);
void initialize_hex(char *hex_file);
void write_to_hex(char *hex_file, char *memory_address_to_write, char *what_to_write, int write_to_dir);
void convert_word_to_instruction(char *file_contents, int size_file_contents);
int hexchar_to_int(char *hex);
char *int_to_hexchar(int number, char *destiny);
#endif // DEALWITHFILE_H
